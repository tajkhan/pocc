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
  s_process_data_t prog_loops[num_for_loops + num_stmts];
  int i, j;
  for (i = 0; i < num_for_loops + num_stmts; ++i)
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
}



static
void traverse_collect_iterators (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_for))
    {
      PAST_DECLARE_TYPED(for, pf, node);
      char** iters = data;
      while (*iters && strcmp (*iters, pf->iterator->symbol->data))
	++iters;
      *iters = pf->iterator->symbol->data;
    }
}

static
char** collect_all_loop_iterators (s_past_node_t* node)
{
  int num_fors = past_count_for_loops (node);
  char** iterators = XMALLOC(char*, num_fors + 1);
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
      pvectorizer_vectorize (program, root);
    }

  // Use PTILE, if asked.
  if (poptions->ptile)
    pocc_driver_ptile (program, root, poptions, puoptions);

  // Insert iterators declaration.
  char** iterators = collect_all_loop_iterators (root);
  int i;
  FILE* body_file = poptions->output_file;
  if (iterators[0])
    fprintf (body_file,"\t register int %s", iterators[0]);
  for (i = 1; iterators[i]; ++i)
    fprintf (body_file,", %s", iterators[i]);
  fprintf (body_file, ";\n\n");
  fflush (body_file);
  fprintf (body_file, "#pragma scop\n");

  /* // Simplify expressions. */
  /* past_simplify_expressions (root); */

  // Pretty-print
  past_pprint_extended_metainfo (body_file, root, metainfoprint, NULL);

  fprintf (body_file, "#pragma endscop\n");

  // Be clean.
  past_deep_free (root);
}
