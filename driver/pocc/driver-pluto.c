/*
 * driver-pluto.c: this file is part of the PoCC project.
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

# include <pocc/driver-pluto.h>
# include <pocc/driver-cloog.h>

// Ugly forward declaration to avoid compilation warning. Would have
// been simpler to create an option.h file in Pluto...
PlutoOptions* pluto_options_alloc ();

int
pocc_driver_pluto (scoplib_scop_p program,
		  s_pocc_options_t* poptions,
		  s_pocc_utils_options_t* puoptions)
{
  if (! poptions->quiet)
    printf ("[PoCC] Running Pluto\n");

  PlutoOptions* ploptions = pluto_options_alloc ();
  ploptions->parallel = poptions->pluto_parallel;
  ploptions->tile = poptions->pluto_tile;
  ploptions->rar = poptions->pluto_rar;
  ploptions->unroll = poptions->pluto_unroll;
  ploptions->fuse = poptions->pluto_fuse;
  ploptions->polyunroll = poptions->pluto_polyunroll;
  ploptions->bee = poptions->pluto_bee;
  ploptions->prevector = poptions->pluto_prevector;
  ploptions->ufactor = poptions->pluto_ufactor; // int
  ploptions->quiet = poptions->pluto_quiet | poptions->quiet;
  ploptions->silent = poptions->quiet;
  ploptions->context = poptions->pluto_context; // int
  ploptions->cloogf = poptions->cloog_f; // int
  ploptions->cloogl = poptions->cloog_l; // int
  ploptions->ft = poptions->pluto_ft; // int
  ploptions->lt = poptions->pluto_lt; // int
  ploptions->multipipe = poptions->pluto_multipipe;
  ploptions->l2tile = poptions->pluto_l2tile;
  ploptions->lastwriter = poptions->pluto_lastwriter;
  ploptions->scalpriv = poptions->pluto_scalpriv;
  ploptions->external_deps = poptions->pluto_external_candl;
  ploptions->candl_simplify_deps = poptions->candl_deps_isl_simplify;
  //ploptions->debug = poptions->verbose;
  /* Set RAR on cost function only option. */
  ploptions->rar_cf = poptions->pluto_rar_cf;
  ploptions->names_are_strings = poptions->names_are_strings;

  ploptions->tiling_in_scattering = poptions->pluto_tiling_in_scatt;
  ploptions->bound_coefficients = poptions->pluto_bound_coefficients;
  
  pocc_options_init_cloog (poptions);
  puoptions->cloog_options = (void*) poptions->cloog_options;

  // Ensure Candl has been run, and scop contains dependence
  // information, if pluto-ext-candl option is set.
  if (poptions->pluto_external_candl)
    {
      char* candldeps =
	scoplib_scop_tag_content (program, "<dependence-polyhedra>",
				  "</dependence-polyhedra>");
      if (candldeps == NULL)
	{
	  // Dependence computation with candl was not done.
	  pocc_driver_candl (program, poptions, puoptions);
	}
      else
	free (candldeps);
    }

  if (pluto_pocc (program, ploptions, puoptions) == EXIT_FAILURE)
    return EXIT_FAILURE;
  poptions->cloog_options = puoptions->cloog_options;

  if (poptions->output_scoplib_file_name)
    {
      scoplib_scop_p tempscop = scoplib_scop_dup (program);
      if (poptions->cloogify_schedules)
	pocc_cloogify_scop (tempscop);
      FILE* scopf = fopen (poptions->output_scoplib_file_name, "w");
      if (scopf)
	{
	  scoplib_scop_print_dot_scop (scopf, tempscop);
	  fclose (scopf);
	}
      scoplib_scop_free (tempscop);
    }

  return EXIT_SUCCESS;
}
