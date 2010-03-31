/*
 * driver-clastops.c: this file is part of the PoCC project.
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

# define CLOOG_SUPPORTS_SCOPLIB
# include <cloog/cloog.h>
# include <cloog/clast.h>
# include <pragmatize/pragmatize.h>
# include <vectorizer/vectorizer.h>
# include <storcompacter/storcompacter.h>
# include <clasttools/pprint.h>
# include <pocc/driver-clastops.h>
# include <clasttools/clastext.h>


static
void
traverse_print_clast_user_statement_extended_defines (struct clast_stmt* s,
						      FILE* out)
{
  // Traverse the clast.
  for ( ; s; s = s->next)
    {
      if (CLAST_STMT_IS_A(s, stmt_for) ||
	  CLAST_STMT_IS_A(s, stmt_parfor) ||
	  CLAST_STMT_IS_A(s, stmt_vectorfor))
	{
	  struct clast_stmt* body;
	  if (CLAST_STMT_IS_A(s, stmt_for))
	    body = ((struct clast_for*)s)->body;
	  else if  (CLAST_STMT_IS_A(s, stmt_parfor))
	    body = ((struct clast_parfor*)s)->body;
	  else if  (CLAST_STMT_IS_A(s, stmt_vectorfor))
	    body = ((struct clast_vectorfor*)s)->body;
	  traverse_print_clast_user_statement_extended_defines (body, out);
	}
      else if (CLAST_STMT_IS_A(s, stmt_guard))
	traverse_print_clast_user_statement_extended_defines
	  (((struct clast_guard*)s)->then, out);
      else if (CLAST_STMT_IS_A(s, stmt_block))
	traverse_print_clast_user_statement_extended_defines
	  (((struct clast_block*)s)->body, out);
      else if (CLAST_STMT_IS_A(s, stmt_user_extended))
	{
	  struct clast_user_stmt_extended* ue =
	    (struct clast_user_stmt_extended*) s;
	  fprintf (out, "%s\n", ue->define_string);
	}
    }
}



void
pocc_driver_clastops (scoplib_scop_p program,
		      CloogProgram* cp,
		      s_pocc_options_t* poptions,
		      s_pocc_utils_options_t* puoptions)
{
  CloogOptions* coptions = poptions->cloog_options;

  /* (1) Create the CLAST associated to the CloogProgram. */
  struct clast_stmt* root = cloog_clast_create (cp, coptions);

  /* (2) Mark parallel loops, if required. */
  if (poptions->vectorizer_mark_par_loops || poptions->storage_compaction)
    {
      s_vectorizer_options_t* voptions = vectorizer_options_malloc ();
      vectorizer_mark_par_loops (program, root, voptions);
      vectorizer_options_free (voptions);
    }

  /* (3) Run storage compaction, if required. */
  if (poptions->storage_compaction)
    {
      if (! poptions->quiet)
	printf ("[PoCC] Running storage compaction\n");
      s_ac_options_t* acoptions = ac_options_malloc ();
      acoptions->keep_outer_par =
	poptions->array_contraction_keep_outer_par_loops;
      acoptions->keep_vectorized =
	poptions->array_contraction_keep_vectorized_loops;
      s_ac_metrics_t* acm =
	storcompacter_array_contraction (program, root, acoptions);
      ac_metrics_print (stdout, acm, program);
      ac_metrics_free (acm);
      ac_options_free (acoptions);
    }

  /* (4) Run the vectorizer, if required. */
  if (poptions->vectorizer)
    {
      if (! poptions->quiet)
	printf ("[PoCC] Running vectorizer\n");
      s_vectorizer_options_t* voptions = vectorizer_options_malloc ();
      voptions->mark_par_loops =
	poptions->vectorizer_mark_par_loops == 1 ? 0 : 1;
      voptions->mark_vect_loops = poptions->vectorizer_mark_vect_loops;
      voptions->vectorize_loops = poptions->vectorizer_vectorize_loops;
      voptions->keep_outer_parallel = poptions->vectorizer_keep_outer_par_loops;
      // Call the vectorizer.
      s_vectorizer_metrics_t* vectm = vectorizer (program, root, voptions);
      vectorizer_metrics_print (stdout, vectm, program);
      vectorizer_metrics_free (vectm);
    }

  /* (5) Run the pragmatizer, if required. */
  if (poptions->pragmatizer)
    {
      if (! poptions->quiet)
	printf ("[PoCC] Insert OpenMP and vectorization pragmas\n");
      pragmatize (program, root);
    }

  /* (6) Generate statements macros. */
  FILE* body_file = poptions->output_file;
  int st_count = 1;
  scoplib_statement_p stm;
  int i;
  for (stm = program->statement; stm; stm = stm->next)
    {
      fprintf (body_file, "#define S%d(", st_count++);
      for (i = 0; i < stm->nb_iterators; ++i)
	{
	  fprintf (body_file, "%s", stm->iterators[i]);
	  if (i < stm->nb_iterators - 1)
	    fprintf (body_file, ",");
	}
      fprintf (body_file, ") %s\n", stm->body);
    }
  /* We now can have statement definition overriden by the array
     contraction. Those are stored in clast_user_statement_extended
     nodes only, the #define is in the cuse->define_string, they must
     be collected and pretty-printed here. */
  traverse_print_clast_user_statement_extended_defines (root, body_file);

  /* (7) Generate loop counters. */
  fprintf (body_file,
	   "\t register int lbv, ubv, lb, ub, lb1, ub1, lb2, ub2;\n");
  int done = 0;
  for (i = 0; i < cp->nb_scattdims; ++i)
    {
      /// FIXME: Deactivate this, as pluto may generate OpenMP pragmas
      /// using some unused variables. We'll let the compiler remove useless
      /// variables.
      if (cp->scaldims[i] == 0 || 1)
	{
	  if (! done++)
	    fprintf (body_file, "\t register int ");
	  else
	    fprintf (body_file, ", ");
	  fprintf(body_file, "c%d, c%dt, newlb_c%d, newub_c%d", i, i, i, i);
	}
    }
  fprintf (body_file, ";\n\n");

  fflush (body_file);
  fprintf (body_file, "#pragma scop\n");

  /* (8) Run the extended CLAST pretty-printer, if needed. */
  if (poptions->pragmatizer || poptions->vectorizer ||
      poptions->vectorizer_mark_par_loops ||
      poptions->vectorizer_mark_vect_loops ||
      poptions->storage_compaction)
    clasttols_clast_pprint_debug (body_file, root, 0, coptions);
  else
    // Pretty-print the code with CLooG default pretty-printer.
    clast_pprint (body_file, root, 0, coptions);

  fprintf (body_file, "#pragma endscop\n");
  
  /// FIXME: This is a BUG: this should be enabled.
/*   /\* (9) Delete the clast. *\/ */
/*   cloog_clast_free (root); */
}
