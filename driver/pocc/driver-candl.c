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
pocc_driver_candl (osl_scop_p program,
		   s_pocc_options_t* poptions,
		   s_pocc_utils_options_t* puoptions)
{
  if (poptions->candl_pass)
    {
      if (! poptions->quiet)
	      printf ("[PoCC] Running Candl\n");

      candl_options_p coptions = candl_options_malloc ();

      osl_scop_p scop = program;
      candl_scop_usr_init(scop);
      while (scop) {

        osl_dependence_p deps = candl_dependence (scop, coptions);
        osl_scop_print(stdout, scop);


#if defined(CANDL_COMPILE_PRUNING_C)
        if (poptions->candl_deps_prune_transcover)
	        deps = candl_dependence_prune_transitively_covered (deps);
#endif
        // Simplify dependences with ISL, if needed.
#ifdef CANDL_SUPPORTS_ISL
        if (poptions->candl_deps_isl_simplify)
	        candl_dependence_isl_simplify(deps, scop);
#endif

        if (poptions->verbose)
	        candl_dependence_pprint (stdout, deps);

        // Embed dependences in the scop, in case we want pluto to read them.
        if (poptions->pluto_external_candl)
        {
          osl_generic_p data = osl_generic_shell(deps,
                                             osl_dependence_interface());
          data->next = scop->extension;
          scop->extension = data;
  	      printf ("[pocc-driver-candl] added dependences to ext\n");
        }
        else
          osl_dependence_free (deps);

        scop = scop->next;
      }
      candl_scop_usr_cleanup(scop);

      candl_options_free (coptions);
    }
}
