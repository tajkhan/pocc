/*
 * pocc.c: this file is part of the PoCC project.
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

#include <stdio.h>
#include "getopts.h"
#include "options.h"




int main(int argc, char** argv)
{
  printf ("PoCC compiler\n");


/*   // Call candl and compute dependences. */
/*   if (options->verbose) */
/*     fprintf (options->out_file, ". Calling candl\n"); */
/*   CandlOptions* candl_opt = candl_options_malloc (); */
/*   /\*   candl_opt->commute = 1; *\/ */
/*   candl_opt->scalar_privatization = 1; */
/*   CandlProgram* program; */
/*   CandlDependence* dependences; */

/*   program = candl_program_read (options->in_file); */
/*   fclose (options->in_file); */


/*   dependences = candl_dependence (program, candl_opt); */

/*   if (options->load_file == NULL) */
/*     { */
/*       // Construct legal transformation space. */
/*       if (options->verbose) */
/* 	fprintf (options->out_file, ". Building legal transformation space\n"); */
/*       space = ls_space (program, dependences, options); */
/*     } */
/*   else */
/*     { */
/*       s_fm_system_t* s; */
/*       space = ls_space_alloc (); */
/*       space->program = program; */
/*       space->dependences = dependences; */
/*       int i; */
/*       space->u_polyhedron = XMALLOC(s_fm_solution_t*, 10); */
/*       space->u_compacted = XMALLOC(s_fm_compsol_t*, 10); */
/*       for (i = 0; i < 10; ++i) */
/* 	{ */
/* 	  space->u_polyhedron[i] = NULL; */
/* 	  space->u_compacted[i] = NULL; */
/* 	} */
/*       char buffer[512]; */
/*       i = 0; */
/*       do */
/* 	{ */
/* 	  sprintf (buffer, "%s_%d.sol", options->load_file, i); */
/* 	  FILE* f = fopen (buffer, "r"); */
/* 	  if (f == NULL) */
/* 	    break; */
/* 	  s = fm_system_read (f); */
/* 	  space->u_polyhedron[i] = fm_system_to_solution (s); */
/* 	  space->u_compacted[i] = fm_compsol_init_sys (s); */
/* 	  fm_system_free (s); */
/* 	  fclose (f); */
/* 	  (space->dimension)++; */
/* 	  ++i; */
/* 	} */
/*       while (1); */
/*       if (space->dimension == 1) */
/* 	space->polyhedron = space->u_polyhedron[0]; */
/*     } */

/*   if (options->heuristic != LS_HEURISTIC_SKIP) */
/*     { */
/*       if (options->verbose) */
/* 	fprintf (options->out_file, */
/* 		 ". Exploring legal transformation space\n"); */
/*       ls_explorer (space, program, options); */
/*     } */

/*   // Be clean. */
/*   if (options->verbose) */
/*     fprintf (options->out_file, ". Cleaning\n"); */
/*   candl_dependence_free (dependences); */
/*   candl_program_free (program); */
/*   candl_options_free (candl_opt); */
/*   pip_close (); */
/*   ls_options_free (options); */
/*   ls_space_free (space); */


}
