/*
 * driver-cloog.c: this file is part of the PoCC project.
 *
 * PoCC, the Polyhedral Compiler Collection package
 *
 * Copyright (C) 2009 Louis-Noel Pouchet
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * The complete GNU General Public Licence Notice can be found as the
 * `COPYING' file in the root directory.
 *
 * Author:
 * Louis-Noel Pouchet <Louis-Noel.Pouchet@inria.fr>
 *
 */
#if HAVE_CONFIG_H
# include <pocc-utils/config.h>
#endif

# include <cloog/cloog.h>
# include <pocc/driver-cloog.h>


void
pocc_driver_cloog (clan_scop_p program,
		  s_pocc_options_t* poptions,
		  s_pocc_utils_options_t* puoptions)
{
  int i;
  FILE* body_file = poptions->output_file;
  printf ("[PoCC] Running CLooG...\n");

  /* Create a CloogProgram from it. */
  CloogOptions* coptions = puoptions->cloog_options;
  CloogProgram * cp = cloog_program_scop_to_cloogprogram (program, coptions);
  /* Store the .scop corresponding to the input program. */
  cp->scop = program;

  /* Generate the code. */
  cp = cloog_program_generate (cp, coptions);

  /* Generate statements macros. */
  int st_count = 1;
  clan_statement_p stm;
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

  /* Generate loop counters. */
  fprintf (body_file, 
	   "\t register int lbv, ubv, lb, ub, lb1, ub1, lb2, ub2;\n");
  int done = 0;
  for (i = 0; i < cp->nb_scattdims; ++i)
    {
      if (cp->scaldims[i] == 0)
	{
	  if (! done)
	    {
	      fprintf (body_file, "\t register int ");
	      done = 1;
	    }
	  else
	    fprintf (body_file, ", ");
	  fprintf(body_file, "c%d, c%dt, newlb_c%d, newub_c%d", i, i, i, i);
	}
    }
  fprintf (body_file, ";\n\n");
  /* Dump the code. */
  cloog_program_pprint (body_file, cp, coptions);
  cloog_program_free (cp);
}
