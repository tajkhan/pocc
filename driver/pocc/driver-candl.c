/*
 * driver-candl.c: this file is part of the PoCC project.
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

# include <pocc/driver-candl.h>


void
pocc_driver_candl (scoplib_scop_p program,
		   s_pocc_options_t* poptions,
		   s_pocc_utils_options_t* puoptions)
{
  if (poptions->candl_pass)
    {
      if (! poptions->quiet)
	printf ("[PoCC] Running Candl\n");
      CandlOptions* coptions = candl_options_malloc ();
      CandlProgram* cprogram = candl_program_convert_scop (program, NULL);
      CandlDependence* deps = candl_dependence (cprogram, coptions);
      if (poptions->verbose)
	candl_dependence_pprint (stdout, deps);
      // Simplify dependences with ISL, if needed.
      if (poptions->candl_deps_isl_simplify)
	  candl_dependence_isl_simplify(deps, cprogram);
      // Embed dependences in the scop, in case we want pluto to read them.
      if (poptions->pluto_external_candl)
	candl_dependence_update_scop_with_deps (program, deps);
      
/*       candl_depvector_extract_in_loop (cprogram, deps, 0); */

      
      candl_dependence_free (deps);
      candl_program_free (cprogram);
      candl_options_free (coptions);
    }
}
