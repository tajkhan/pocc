/*
 * driver-ptile.c: this file is part of the PoCC project.
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

# include <pocc/driver-ptile.h>


# ifndef POCC_RELEASE_MODE
#  include <parametrictiling/options.h>
#  include <parametrictiling/parametrictiling.hpp>
# endif

# include <candl/candl.h>
# include <candl/ddv.h>
# include <scoptools/past2scop.h>


struct s_subscop
{
  s_past_node_t* root;
  scoplib_scop_p scop;
};
typedef struct s_subscop s_subscop_t;

/**
 * past_macro_stmt
 *
 *
 */

struct past_macro_stmt_t
{
  s_past_node_t		node;
  //
  s_past_node_t*	body;
};
typedef struct past_macro_stmt_t s_past_macro_stmt_t;
PAST_DECLARE_NODE_IN_HIERARCHY_HEADER(macro_stmt);


static void
past_macro_stmt_free (s_past_node_t* node)
{
  assert (past_node_is_a (node, past_macro_stmt));
  s_past_macro_stmt_t* pf = (s_past_macro_stmt_t*) node;
  past_deep_free (pf->body);
  XFREE(pf);
}
static void
past_macro_stmt_visitor (s_past_node_t* node,
			past_fun_t prefix,
			void* prefix_data,
			past_fun_t suffix,
			void* suffix_data)
{
  assert (past_node_is_a (node, past_macro_stmt));
  PAST_DECLARE_TYPED(macro_stmt, r, node);
  past_visitor (r->body, prefix, prefix_data, suffix, suffix_data);
}
PAST_DECLARE_NODE_IN_HIERARCHY_UNIT_1(macro_stmt);

s_past_macro_stmt_t* past_macro_stmt_create (s_past_node_t* body)
{
  s_past_macro_stmt_t* n = XMALLOC(s_past_macro_stmt_t, 1);
  n->node.type = past_macro_stmt;
  n->node.visitor = past_macro_stmt_visitor;
  n->node.parent = NULL;
  n->node.next = NULL;
  n->body = body;
  for (; body; body->parent = &(n->node), body = body->next)
    ;

  return n;
}

s_past_node_t* past_node_macro_stmt_create (s_past_node_t* body)
{
  return &past_macro_stmt_create (body)->node;
}


static
int
loops_are_nested (CandlProgram* cprogram, int l1, int l2)
{
  int i, j;

  for (i = 0; i < cprogram->nb_statements; ++i)
    {
      int nested = 0;
      for (j = 0; j < cprogram->statement[i]->depth; ++j)
	{
	  int lidx = cprogram->statement[i]->index[j];
	  if (lidx == l1 || lidx == l2)
	    ++nested;
	}
      if (nested == 2)
	return 1;
    }

  return 0;
}


static
int
is_outer_for_loop (s_past_node_t* node)
{
  for (node = node->parent; node; node = node->parent)
    if (past_node_is_a (node, past_for))
      return 0;

  return 1;
}

static
void
count_for_loops (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_for))
    (*((int*)data))++;
}

struct s_process_data
{
  s_past_node_t*	fornode;
  int			forid;
  int			is_outer;
};
typedef struct s_process_data s_process_data_t;

static
void traverse_tree_index_for (s_past_node_t* node, void* data)
{
  if (past_node_is_a(node, past_for))
    {
      int i;
      s_process_data_t* pd = (s_process_data_t*) data;
      for (i = 0; pd[i].fornode != NULL; ++i)
	;
      pd[i].fornode = node;
      pd[i].forid = i;
      pd[i].is_outer = is_outer_for_loop (node);
    }
}


#ifndef max
# define max(a,b) (a < b ? b : a)
#endif

static
void compute_loop_depth (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_for))
    {
      void** mydata = (void**)data;
      int depth = 1;
      while (node && node != mydata[0])
	{
	  if (past_node_is_a (node, past_for))
	    depth++;
	  node = node->parent;
	}
      int olddepth = *((int*)mydata[1]);
      int newdepth = max(olddepth, depth);
      *((int*)mydata[1]) = newdepth;
    }
}


static
void traverse_get_max_loop_depth (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_cloogstmt))
    {
      s_past_node_t* top = ((void**)data)[0];
      int* maxdepth = ((void**)data)[2];
      int depth = 0;
      s_past_node_t* curnode = node;
      for (; node; node = node->parent)
	{
	  if (past_node_is_a (node, past_for))
	    ++depth;
	  if (node == top)
	    break;
	}
      if (depth > *maxdepth)
	{
	  ((void**)data)[1] = curnode;
	  *maxdepth = depth;
	}
    }
}

static
char** compute_iterator_list (s_past_node_t* root)
{
  // Compute the max_depth stmt.
  void* args[3];
  args[0] = root;
  args[1] = NULL;
  int maxdepth = 0;
  args[2] = &maxdepth;
  past_visitor (root, traverse_get_max_loop_depth, (void*)args, NULL, NULL);

  s_past_node_t* node;
  char** ret = XMALLOC(char*, maxdepth + 1);
  int pos = 0;
  for (node = args[1]; node; node = node->parent)
    {
      if (past_node_is_a(node, past_for))
	{
	  PAST_DECLARE_TYPED(for, pf, node);
	  ret[pos++] = pf->iterator->symbol->data;
	  if (node == root)
	    break;
	}
    }
  ret[pos] = NULL;

  return ret;
}

static
void visit_create_otl_loops (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_cloogstmt))
    {
      s_past_node_t* top = ((void**)(data))[0];
      int maxdepth = *((int*)(((void**)data)[1]));
      // Count the number of surrounding for loops.
      s_past_node_t* p;
      int cur_for = 0;
      char* iters[maxdepth + 1];
      // top is a past_for.
      for (p = node; p && p != top->parent; p = p->parent)
	{
	  if (past_node_is_a (p, past_for))
	    {
	      PAST_DECLARE_TYPED(for, pf, p);
	      iters[cur_for++] = pf->iterator->symbol->data;
	    }
	}
      iters[cur_for] = 0;
      s_past_node_t* curforbody = node;
      s_past_node_t** target = past_node_get_addr (node);
      int i, j;
      char** loop_iters = iters;
      char** alliters = ((void**)data)[3];
      char** all_iters = alliters;
      int pos_iter = 0;
      for (i = 0; i < maxdepth - cur_for; ++i)
	{
	  while (*loop_iters && ! strcmp (*loop_iters, *all_iters))
	    {
	      ++loop_iters;
	      ++all_iters;
	    }
	  char* buffer = *all_iters;
	  s_symbol_t* itersymb = symbol_add_from_char (NULL, buffer);
	  s_past_node_t* init =
	    past_node_binary_create (past_assign,
				     past_node_variable_create (itersymb),
				     past_node_value_create_from_int (0));
	  itersymb = symbol_add_from_char (NULL, buffer);
	  s_past_node_t* test =
	    past_node_binary_create (past_leq,
				     past_node_variable_create (itersymb),
				     past_node_value_create_from_int (0));
	  itersymb = symbol_add_from_char (NULL, buffer);
	  s_past_node_t* increment =
	    past_node_unary_create (past_inc_before,
				    past_node_variable_create (itersymb));
	  itersymb = symbol_add_from_char (NULL, buffer);
	  s_past_variable_t* iter = past_variable_create (itersymb);

	  s_past_node_t* newfor =
	    past_node_for_create (init, test, iter,
				  increment, curforbody, NULL);
 	  newfor->next = curforbody->next;
	  *target = newfor;
	  curforbody->next = NULL;
	  curforbody = newfor;
	  ((void**)(data))[2] = curforbody;
	  ++all_iters;
	}
    }
}


void
create_otl_loops (s_past_node_t* node)
{
  // 1- Compute the maximal loop depth.
  void* data[4];
  data[0] = node;
  int max_depth = 0;
  data[1] = &max_depth;
  past_visitor (node, compute_loop_depth, (void*)data, NULL, NULL);
  data[3] = compute_iterator_list (node);

  // 2- Iterate on all statements
  do
    {
      data[2] = NULL;
      past_visitor (node, visit_create_otl_loops, (void*)data, NULL, NULL);
      past_set_parent (node);
    }
  while (data[2]);
}

s_subscop_t*
pocc_create_tilable_nests (scoplib_scop_p program,
			   s_past_node_t* root)
{
  // Create SCoP corresponding to the transformed code.
  scoplib_scop_p newscop = scoptools_past2scop (root, program);
  CandlOptions* coptions = candl_options_malloc ();
  CandlProgram* cprogram = candl_program_convert_scop (newscop, NULL);
  CandlDependence* cdeps = candl_dependence (cprogram, coptions);

  int num_for_loops = 0;
  past_visitor (root, count_for_loops, (void*)&num_for_loops, NULL, NULL);

  s_subscop_t* ret = XMALLOC(s_subscop_t, num_for_loops);
  s_process_data_t prog_loops[num_for_loops];
  int i, j;
  for (i = 0; i < num_for_loops; ++i)
    prog_loops[i].fornode = NULL;
  past_visitor (root, traverse_tree_index_for, (void*)prog_loops, NULL, NULL);

  int partid = 0;
  for (i = 0; i < num_for_loops; ++i)
    {
      if (prog_loops[i].is_outer)
	{
	  for (j = i + 1; j < num_for_loops; ++j)
	    if (loops_are_nested (cprogram, i, j) &&
		! candl_loops_are_permutable (cprogram, cdeps, i, j))
	      break;
	  if (j == num_for_loops)
	    {
	      // All loops in the nest are permutable. Process it.
	      ret[partid].root = prog_loops[i].fornode;

	      // Do 'otl' on the loop nest.
	      create_otl_loops (ret[partid].root);
	      scoplib_scop_free (newscop);
	      newscop = scoptools_past2scop (root, program);
	      ret[partid].scop = newscop;
	      ++partid;
	    }
	}
    }
  ret[partid].root = NULL;
  if (partid == 0)
    {
      printf ("[PoCC][Warning] There is no fully permutable loop nest in the program\n");
      scoplib_scop_free (newscop);
    }
  // Be clean.
  candl_dependence_free (cdeps);
  candl_program_free (cprogram);

  return ret;
}

void
pocc_expand_macro_stmt (s_past_node_t* root)
{

}


static
void
update_sym_table (s_past_node_t* node, void* data)
{
  if (past_node_is_a (node, past_variable))
    {
      s_symbol_table_t* table = (s_symbol_table_t*)data;
      PAST_DECLARE_TYPED(variable, pv, node);
      if (! symbol_find (table, pv->symbol))
	{
	  s_symbol_t* s;
	  if (pv->symbol->is_char_data)
	    s = symbol_add_from_char (table, pv->symbol->data);
	  else
	    s = symbol_add_from_data (table, pv->symbol->data);
	  symbol_free (pv->symbol);
	  pv->symbol = s;
	}
    }
}



void
pocc_driver_ptile (scoplib_scop_p program,
		   s_past_node_t* root,
		   s_pocc_options_t* poptions,
		   s_pocc_utils_options_t* puoptions)
{
# ifndef POCC_RELEASE_MODE

  printf ("[PoCC] Use parametric tiling\n");

  // Set parent, just in case.
  past_set_parent (root);
  // Extract tileable components.
  s_subscop_t* tileable_comps = pocc_create_tilable_nests (program, root);

  // Iterate on all tileable components, parametrically tile them.
  s_parametrictiling_options_t* ptopts = parametrictiling_options_malloc ();
  ptopts->RSFME = 1;
  ptopts->fullTileSeparation = 0;
  ptopts->verbose_level = 1;
  int i;
  for (i = 0; tileable_comps[i].root; ++i)
    {
      s_past_node_t** addr = past_node_get_addr (tileable_comps[i].root);
      s_past_node_t* newpast =
	parametricallytile (tileable_comps[i].scop,
			    tileable_comps[i].root, ptopts);
      newpast->next = tileable_comps[i].root->next;
      assert (addr);
      *addr = newpast;
      pocc_expand_macro_stmt (newpast);
    }

  // Update the symbol table.
  s_symbol_table_t* table = ((s_past_root_t*)root)->symboltable;
  past_visitor (root, update_sym_table, (void*)table, NULL, NULL);

  // Be clean.
  if (tileable_comps[0].root != NULL)
    scoplib_scop_free (tileable_comps[0].scop);
  XFREE(tileable_comps);
  parametrictiling_options_free (ptopts);


# endif
}

