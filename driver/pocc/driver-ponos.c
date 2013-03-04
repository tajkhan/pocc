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

# include <pocc/driver-ponos.h>
//# include <pocc/driver-cloog.h>



int
pocc_driver_ponos (scoplib_scop_p program,
		  s_pocc_options_t* poptions,
		  s_pocc_utils_options_t* puoptions)
{
  if (! poptions->quiet)
    printf ("[PoCC] Running Ponos\n");

  s_ponos_options_t* popts = ponos_options_malloc ();
  popts->debug = poptions->ponos_debug;
  popts->build_2d_plus_one = poptions->ponos_build_2d_plus_one;
  popts->maxscale_solver = poptions->ponos_maxscale_solver;
  popts->noredundancy_solver = poptions->ponos_noredundancy_solver;
  popts->legality_constant = poptions->ponos_legality_constant_K;
  popts->schedule_bound = poptions->ponos_schedule_bound;
  popts->schedule_size = poptions->ponos_schedule_dim;
  popts->solver = poptions->ponos_solver_type;
  popts->solver_precond = poptions->ponos_solver_precond;
  popts->quiet = poptions->ponos_quiet;
  popts->schedule_coefs_are_pos = poptions->ponos_coef_are_pos;
  popts->objective = poptions->ponos_objective;

  //
  ponos_scheduler (program, popts);

  // Dump the scop file, if needed.
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


  ponos_options_free (popts);

  return EXIT_SUCCESS;
}
