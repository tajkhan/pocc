/*
 * driver-pastops.c: this file is part of the PoCC project.
 *
 * PoCC, the Polyhedral Compiler Collection package
 *
 * Copyright (C) 2009 Louis-Noel Pouchet
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * The complete GNU General Public Licence Notice can be found as the
 * `COPYING.LESSER' file in the root directory.
 *
 * Author:
 * Louis-Noel Pouchet <Louis-Noel.Pouchet@inria.fr>
 *
 */
#if HAVE_CONFIG_H
# include <pocc-utils/config.h>
#endif

#include <pocc/driver-pastops.h>
#include <past/past_api.h>
#include <past/pprint.h>
#include <irconverter/past2scop.h>
#include <candl/candl.h>

#include <pvectorizer/vectorize.h>
#include <punroller/punroll.h>


struct s_process_data
{
  s_past_node_t*	fornode;
  int			forid;
  int			is_parallel;
};
typedef struct s_process_data s_process_data_t;

static
void traverse_tree_index_for (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_for))
    {
      int i;
      s_process_data_t* pd = (s_process_data_t*) data;
      for (i = 0; pd[i].fornode != NULL; ++i)
	;
      pd[i].fornode = node;
      pd[i].forid = i;
      pd[i].is_parallel = 0;
    }
  if (past_node_is_a (node, past_cloogstmt))
    {
      // Special case: statements not surrouded by any loop in the
      // tree that are surrounded by a fake loop in the scop representation.
      s_past_node_t* parent;
      for (parent = node->parent; parent && !past_node_is_a (parent, past_for);
	   parent = parent->parent)
	;
      if (!parent)
	{
	  int i;
	  s_process_data_t* pd = (s_process_data_t*) data;
	  for (i = 0; pd[i].fornode != NULL; ++i)
	    ;
	  pd[i].fornode = node;
	  pd[i].forid = -i;
	}
    }
}


/**
 * Translate all past_for nodes which are sync-free parallel into
 * past_parfor.
 *
 */
static
void
translate_past_for (scoplib_scop_p original_scop,
		    s_past_node_t* root,
		    int data_is_char)
{
  // 1- Get the scop representation of the tree.
  scoplib_scop_p scop =
    past2scop_control_only (root, original_scop, data_is_char);
  CandlOptions* coptions = candl_options_malloc ();
  //coptions->scalar_privatization = 1;
  coptions->verbose = 1;
  CandlProgram* cprogram = candl_program_convert_scop (scop, NULL);
  CandlDependence* cdeps = candl_dependence (cprogram, coptions);
  int num_for_loops = past_count_for_loops (root);
  int num_stmts = past_count_statements (root);
  // Oversize the data structure, to deal with fake iterators.
  s_process_data_t prog_loops[num_for_loops + num_stmts + 1];
  int i, j;
  for (i = 0; i < num_for_loops + num_stmts + 1; ++i)
    prog_loops[i].fornode = NULL;
  past_visitor (root, traverse_tree_index_for, (void*)prog_loops, NULL, NULL);

  // Recompute the number of actual for loops.
  for (num_for_loops = 0; prog_loops[num_for_loops].fornode; ++num_for_loops)
    ;

  // 2- Iterate on all loops.
  for (i = 0; i < num_for_loops; ++i)
    {
      // Skip fake loops.
      if (prog_loops[i].forid < 0)
	continue;
      // Check if a dependence is loop-carried.
      CandlDependence* d;
      for (d = cdeps; d; d = d->next)
	if (candl_dependence_is_loop_carried (cprogram, d, i))
	  break;
      if (d == NULL && past_node_is_a (prog_loops[i].fornode, past_for))
	// The loop is sync-free parallel, translate it to past_parfor.
	past_for_to_parfor (prog_loops[i].fornode);
    }
  
  candl_dependence_free (cdeps);
  candl_program_free (cprogram);
  candl_options_free (coptions);
  scoplib_scop_shallow_free (scop);
  past_set_parent (root);
}



static
void traverse_collect_iterators (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_for))
    {
      PAST_DECLARE_TYPED(for, pf, node);
      s_symbol_t** iters = data;
      int i;
      for (i = 0; iters[i] && !symbol_equal (iters[i], pf->iterator->symbol);
	   ++i)
	;
      if (! iters[i])
	iters[i] = pf->iterator->symbol;
    }
  else if (past_node_is_a (node, past_statement))
    {
      PAST_DECLARE_TYPED(statement, ps, node);
      // Special case of otl loops.
      if (past_node_is_a (ps->body, past_assign))
	{
	  PAST_DECLARE_TYPED(binary, pb, ps->body);
	  if (past_node_is_a (pb->lhs, past_variable))
	    {
	      PAST_DECLARE_TYPED(variable, pv, pb->lhs);
	      if (pv->symbol->is_char_data &&
		  ((char*)pv->symbol->data)[0] == 'c')
		{
		  s_symbol_t** iters = data;
		  int i;
		  for (i = 0; iters[i] &&
			 !symbol_equal (iters[i], pv->symbol); ++i)
		    ;
		  if (! iters[i])
		    iters[i] = pv->symbol;
		}
	    }
	}
    }
}

static
s_symbol_t** collect_all_loop_iterators (s_past_node_t* node)
{
  int num_fors = past_count_for_loops (node);
  s_symbol_t** iterators = XMALLOC(s_symbol_t*, num_fors + 1);
  int i;
  for (i = 0; i <= num_fors; ++i)
    iterators[i] = NULL;

  past_visitor (node, traverse_collect_iterators, iterators, NULL, NULL);

  return iterators;
}

static
void metainfoprint (s_past_node_t* node, FILE* out)
{
  if (node->metainfo)
    fprintf (out, "%s", (char*) node->metainfo);
}

static
void traverse_expr_for_tile (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_mul))
    {
      PAST_DECLARE_TYPED (binary, pb, node);
      if (past_node_is_a (pb->lhs, past_value) &&
	  past_node_is_a (pb->rhs, past_variable))
	{
	  PAST_DECLARE_TYPED(variable, pv, pb->rhs);
	  PAST_DECLARE_TYPED(value, pu, pb->rhs);
	  void** args = (void**)data;
	  int i;
	  s_symbol_t** outer_iters = (s_symbol_t**) args[0];
	  for (i = 0; outer_iters[i] && outer_iters[i] != pv->symbol; ++i)
	    ;
	  if (outer_iters && pu->type == e_past_value_int &&
	      pu->value.intval > 4)
	    {
	      int* ret = (int*) args[1];
	      *ret = 1;
	      args[2] = pv->symbol;
	    }
	}

    }
}


static
void traverse_mark_loop_type (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_for))
    {
      // If we can find an expression of the form 'x * outer_iterator' in
      // the loop bound, x > some value (say, 4) then 'outer_iterator'
      // is a tile loop and the loop is a point loop.

      // a- Collect surrounding loops.
      int count = 0;
      s_past_node_t* parent;
      for (parent = node->parent; parent; parent = parent->parent)
	if (past_node_is_a (parent, past_for))
	  ++count;
      s_symbol_t* outer_iters[count + 1];
      for (count = 0, parent = node->parent; parent; parent = parent->parent)
	if (past_node_is_a (parent, past_for))
	  {
	    PAST_DECLARE_TYPED(for, pf, parent);
	    outer_iters[count++] = pf->iterator->symbol;
	  }
      outer_iters[count] = NULL;
      int is_pt_loop = 0;
      void* data[3];
      data[0] = outer_iters;
      data[1] = &is_pt_loop;
      data[2] = NULL;
      past_visitor (node, traverse_expr_for_tile, (void*)data,
		    NULL, NULL);
      PAST_DECLARE_TYPED(for, pf, node);
      if (is_pt_loop)
	{
	  pf->type = e_past_point_loop;
	  for (parent = node->parent; parent; parent = parent->parent)
	    if (past_node_is_a (parent, past_for))
	      {
		PAST_DECLARE_TYPED(for, pf2, parent);
		if (pf2->iterator->symbol == data[3])
		  pf2->type = e_past_tile_loop;
	      }
	}
    }
}

static
void traverse_collect_ploop (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_for))
    {
      PAST_DECLARE_TYPED(for, pf, node);
      // Collect both point loop nests and untiled loop nests.
      if (pf->type == e_past_point_loop ||
	  pf->type == e_past_unknown_loop)
	{
	  int i;
	  s_past_node_t** args = (s_past_node_t**)data;
	  for (i = 0; args[i]; ++i)
	    ;
	  args[i] = node;
	}
    }
}

static
void traverse_mark_loop (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_for))
    {
      s_past_node_t** marked = (s_past_node_t**)data;
      int i;
      for (i = 0; marked[i]; ++i)
	;
      marked[i] = node;
    }
}

static
s_past_node_t**
collect_point_loop_components (s_past_node_t* root)
{
  // Collect all point loops.
  int num_loops = past_count_for_loops (root);
  s_past_node_t* ploops[num_loops + 1];
  s_past_node_t* marked_ploops[num_loops + 1];
  s_past_node_t* nests[num_loops + 1];
  int i, j;
  int num_nest;
  for (i = 0; i <= num_loops; ++i)
    ploops[i] = marked_ploops[i] = nests[i] = NULL;
  past_visitor (root, traverse_collect_ploop, ploops, NULL, NULL);

  // Inspect the point loops, extract all loop nests.
  for (i = 0; ploops[i]; ++i)
    {
      // Ensure the loop was not already marked as processed.
      for (j = 0; marked_ploops[j] && marked_ploops[j] != ploops[i]; ++j)
	;
      if (marked_ploops[j])
	continue;

      // See if it is an outer point loop.
      s_past_node_t* parent;
      for (parent = ploops[i]->parent; parent &&
	     !past_node_is_a (parent, past_for); parent = parent->parent)
	;
      if (parent)
	{
	  PAST_DECLARE_TYPED(for, pf, parent);
	  if (pf->type == e_past_point_loop)
	    continue;
	}
      for (num_nest = 0; nests[num_nest]; ++num_nest)
	;
      nests[num_nest] = ploops[i];
      // Mark all loops in the nest.
      s_past_node_t* next = ploops[i]->next;
      ploops[i]->next = NULL;
      past_visitor (ploops[i], traverse_mark_loop, marked_ploops,
		    NULL, NULL);
      ploops[i]->next = next;
    }

  s_past_node_t** ret = XMALLOC(s_past_node_t*, num_nest + 2);
  for (i = 0; i <= num_nest; ++i)
    ret[i] = nests[i];
  ret[i] = NULL;

  return ret;
}

static
void traverse_rename_expr (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_mul))
    {
      PAST_DECLARE_TYPED(binary, pb, node);
      if (past_node_is_a (pb->lhs, past_variable) &&
	  past_node_is_a (pb->rhs, past_variable))
	{
	  PAST_DECLARE_TYPED(variable, pv1, pb->lhs);
	  PAST_DECLARE_TYPED(variable, pv2, pb->rhs);
	  s_symbol_t* s1 = pv1->symbol;
	  s_symbol_t* s2 = pv2->symbol;
	  void** args = (void**)data;
	  s_symbol_t** iterators = args[0];
	  s_past_node_t*** map = args[1];
	  int i, j;
	  for (i = 0; iterators[i]; ++i)
	    if (symbol_equal (iterators[i], s1) ||
		symbol_equal (iterators[i], s2))
	      break;
	  if (! iterators[i])
	    {
	      char buffer[1024];
	      // Build a fake symbol.
	      if (s1->is_char_data)
		strcpy (buffer, (char*)s1->data);
	      else
		sprintf (buffer, "%p", s1->data);
	      if (s2->is_char_data)
		strcat (buffer, (char*)s2->data);
	      else
		sprintf (buffer, "%s%p", buffer, s1->data);
	      s_symbol_t* fake_symbol = symbol_add_from_char (NULL, buffer);
	      s_past_node_t* fakevar = past_node_variable_create (fake_symbol);
	      // Insert into the translation map.
	      for (j = 0; map[j][0]; ++j)
		;
	      map[j][0] = fakevar;
	      map[j][1] = node;
	    }
	}
    }
}

static
void
convert_non_affine_expr (s_past_node_t* node, s_past_node_t*** map)
{
  // Collect loop iterators in the nest.
  s_symbol_t** iterators = collect_all_loop_iterators (node);

  // All multiplications of variable expressions that do not involve a
  // loop iterator can be safely promoted to an affine expression, via
  // a fake parameter.
  // Fill-in the replacement map.
  void* args[2]; args[0] = iterators; args[1] = map;
  past_visitor (node, traverse_rename_expr, (void*)args, NULL, NULL);

  // Replace expressions.
  int i;
  for (i = 0; map[i][0]; ++i)
    past_replace_node (map[i][1], map[i][0]);

  XFREE(iterators);
}

static
scoplib_matrix_p extend_matrix (scoplib_matrix_p mat, int nb_par)
{
  scoplib_matrix_p ret =
    scoplib_matrix_malloc (mat->NbRows, mat->NbColumns + nb_par);
  int i, j;
  for (i = 0; i < mat->NbRows; ++i)
    {
      for (j = 0; j < mat->NbColumns - 1; ++j)
	SCOPVAL_assign(ret->p[i][j], mat->p[i][j]);
      SCOPVAL_assign(ret->p[i][ret->NbColumns - 1], mat->p[i][j]);
    }
  scoplib_matrix_free (mat);

  return ret;
}

static
scoplib_matrix_list_p extend_matrix_list (scoplib_matrix_list_p list,
					  int nb_par)
{
  scoplib_matrix_list_p ret = list;
  for (; list; list = list->next)
    list->elt = extend_matrix (list->elt, nb_par);

  return ret;
}

static
scoplib_scop_p create_mapped_scop (scoplib_scop_p program, s_past_node_t*** map)
{
  scoplib_scop_p newscop = scoplib_scop_dup (program);

  // Create unique list of fake parameters.
  int i, j, k;
  for (i = 0; map[i][0]; ++i)
    ;
  char* uniquefparam[i + 1];
  for (i = 0; map[i][0]; ++i)
    uniquefparam[i] = NULL;
  uniquefparam[i] = NULL;

  for (i = 0; map[i][0]; ++i)
    {
      PAST_DECLARE_TYPED(variable, pv, map[i][0]);
      for (j = 0; uniquefparam[j]; ++j)
	if (! strcmp ((char*)pv->symbol->data, uniquefparam[j]))
	  break;
      if (! uniquefparam[j])
	uniquefparam[j] = (char*)pv->symbol->data;
    }

  // Insert the parameters in the scop.
  for (j = 0; uniquefparam[j]; ++j)
    ;
  int count = j;
  char** newparameters = XMALLOC(char*, newscop->nb_parameters + count + 1);
  for (j = 0; j < newscop->nb_parameters; ++j)
    newparameters[j] = newscop->parameters[j];
  for (k = 0; uniquefparam[k]; ++k)
    newparameters[j + k] = uniquefparam[k];
  newparameters[newscop->nb_parameters + count] = NULL;
  newscop->nb_parameters += count;
  XFREE(newscop->parameters);
  newscop->parameters = newparameters;

  // Extend all the scop matrices to contain the new parameters.
  newscop->context = extend_matrix (newscop->context, count);
  scoplib_statement_p stm;
  for (stm = newscop->statement; stm; stm = stm->next)
    {
      stm->domain = extend_matrix_list (stm->domain, count);
      stm->read = extend_matrix (stm->read, count);
      stm->write = extend_matrix (stm->write, count);
      stm->schedule = extend_matrix (stm->schedule, count);
    }

  return newscop;
}


static
void restore_non_affine_expr (s_past_node_t*** map)
{
  // Replace expressions.
  int i;
  for (i = 0; map[i][0]; ++i)
    {
      past_replace_node (map[i][0], map[i][1]);
      past_deep_free (map[i][0]);
    }
}

static
void post_vectorize (s_past_node_t* root, scoplib_scop_p program,
		     int keep_outer_par)
{
  // Set parent, just in case.
  past_set_parent (root);

  // Collect all point-loop nests.
  s_past_node_t** nests = collect_point_loop_components (root);

  // For each of them, apply vectorization.
  // Note: pvectorizer will ignore otl loops as candidates for
  // sinking.
  int i, j, k;
  for (i = 0; nests[i]; ++i)
    {
      s_past_node_t* next = nests[i]->next;
      nests[i]->next = NULL;

      // Convert non-affine expression into a fake affine one.
      int nb_mult = past_count_nodetype (nests[i], past_mul);
      s_past_node_t*** map = XMALLOC(s_past_node_t**, nb_mult + 1);
      for (j = 0; j <= nb_mult; ++j)
	{
	  map[j] = XMALLOC(s_past_node_t*, 2);
	  map[j][0] = map[j][1] = NULL;
	}
      convert_non_affine_expr (nests[i], map);

      // Update the scop with the new fake parameters.
      scoplib_scop_p newscop = create_mapped_scop (program, map);
      // Vectorize loops.
      pvectorizer_vectorize (newscop, nests[i], keep_outer_par);

      // Restore non-affine expressions. Destroy the map entries.
      restore_non_affine_expr (map);

      // Be clean.
      for (j = 0; j <= nb_mult; ++j)
	XFREE(map[j]);
      XFREE(map);
      scoplib_scop_free (newscop);
      nests[i]->next = next;
    }
}



/**
 * PAST post-processing and pretty-printing.
 *
 */
void
pocc_driver_pastops (scoplib_scop_p program,
		     s_past_node_t* root,
		     s_pocc_options_t* poptions,
		     s_pocc_utils_options_t* puoptions)
{
  if (! poptions->quiet)
    printf ("[PoCC] Using the PAST back-end\n");

  // Set parent, just in case.
  past_set_parent (root);

  // Mark the loop type.
  past_visitor (root, traverse_mark_loop_type, NULL, NULL, NULL);

  // Translate parallel for loops into parfor loops.
  if (poptions->pragmatizer)
    translate_past_for (program, root, 1);

  // Pre-vectorize.
  if (poptions->vectorizer)
    {
      if (! poptions->quiet)
	printf ("[PoCC] Move vectorizable loop(s) inward\n");
      pvectorizer_vectorize (program, root, poptions->ptile);
    }

  // Use PTILE, if asked.
  if (poptions->ptile)
    {
      pocc_driver_ptile (program, root, poptions, puoptions);
      // Post-vectorize.
      if (poptions->vectorizer)
	{
	  if (! poptions->quiet)
	    printf ("[PoCC] Post-vectorization: move vectorizable loop(s) inward\n");
	  post_vectorize (root, program, 0);
	}
    }

  /* // Simplify expressions. */
  /* past_simplify_expressions (root); */

  // Use Punroller, if asked.
  if (poptions->punroll && ! poptions->punroll_and_jam)
    {
      if (! poptions->quiet)
	printf ("[PoCC] Perform inner loop unrolling (factor=%d)\n",
		poptions->punroll_size);
      punroll (program, root, poptions->punroll_size,
	       poptions->nb_registers);
    }
  if (poptions->punroll_and_jam)
    {
      if (! poptions->quiet)
	printf ("[PoCC] Perform unroll-and-jam (factor=%d)\n",
		poptions->punroll_size);
      punroll_and_jam (program, root, NULL, poptions->nb_registers);
    }

  // Systematically optimize the loop bounds (hoisting).
  past_optimize_loop_bounds (root);

  // Insert iterators declaration.
  s_symbol_t** iterators = collect_all_loop_iterators (root);
  int i;
  FILE* body_file = poptions->output_file;
  if (iterators[0])
    {
      if (iterators[0]->is_char_data)
	fprintf (body_file,"\t register int %s", (char*) iterators[0]->data);
      for (i = 1; iterators[i]; ++i)
	if (iterators[i]->is_char_data)
	  fprintf (body_file,", %s", (char*)iterators[i]->data);
      fprintf (body_file, ";\n\n");
      fflush (body_file);
    }
  fprintf (body_file, "#pragma scop\n");
  
  // Pretty-print
  past_pprint_extended_metainfo (body_file, root, metainfoprint, NULL);

  fprintf (body_file, "#pragma endscop\n");

  // Be clean.
  past_deep_free (root);
  XFREE(iterators);
}
