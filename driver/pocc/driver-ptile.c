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

#include <pocc/driver-ptile.h>


#include <ptile/options.h>
#include <ptile/pocc_driver.h>
#include <irconverter/past2scop.h>

void
pocc_driver_ptile (scoplib_scop_p program,
		   s_past_node_t* root,
		   s_pocc_options_t* poptions,
		   s_pocc_utils_options_t* puoptions)
{
  if (! poptions->quiet)
    printf ("[PoCC] Use parametric tiling\n");

  // Set parent, just in case.
  past_set_parent (root);

  // Extract scoplib representation.
  scoplib_scop_p control_scop = past2scop_control_only (root, program, 1);
  s_ptile_options_t* ptopts = ptile_options_malloc ();
  ptopts->fullTileSeparation = 0;
  ptopts->verbose_level = 1;
  ptopts->data_is_char = 1;
  ptopts->quiet = poptions->quiet;
  if (ptopts->quiet)
    ptopts->verbose_level = 0;
  
  // Invoke PTile main driver. Will parametrically tile all tilable
  // components, by in-place modification of 'root'.
  ptile_pocc_driver (control_scop, root, ptopts);

  // Be clean.
  scoplib_scop_free (control_scop);
  ptile_options_free (ptopts);
}

