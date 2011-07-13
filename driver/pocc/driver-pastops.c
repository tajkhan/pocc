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
      if (d == NULL)
	// The loop is sync-free parallel, translate it to past_parfor.
	prog_loops[i].fornode->type = past_parfor;
    }
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
  // Translate parallel for loops into parfor loops.
  if (!poptions->ptile)
    translate_past_for (program, root, 1);

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
  past_pprint_metainfo (body_file, root, metainfoprint);

  fprintf (body_file, "#pragma endscop\n");

  // Be clean.
  past_deep_free (root);
}
