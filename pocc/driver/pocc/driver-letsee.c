/*
 * driver-letsee.c: this file is part of the PoCC project.
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

# include <pocc/driver-letsee.h>
# include <letsee/pocc-driver.h>

void 
pocc_driver_letsee (void* program, 
		    s_pocc_options_t* poptions,
		    s_pocc_utils_options_t* puoptions)
{
  printf ("LetSee\n");
  s_ls_options_t* options = ls_options_malloc ();

  options->type = poptions->letsee_space;
  options->create_schedfiles = poptions->codegen;
  options->transfo_dir = strdup ("letsee-transformations");
  options->heuristic = poptions->letsee_traversal;
  options->scheme_m1 = poptions->letsee_scheme_m1;
  options->backtrack_mode = poptions->letsee_backtrack_multi;
  options->prune_oset = poptions->letsee_prune_precut;
  options->normalize_space = poptions->letsee_normspace;
  //options->thresold = poptions->letsee_thresold;
  options->rtries = poptions->letsee_rtries;

/*   letsee_pocc (program, options, puoptions, pocc_codegen); */

  ls_options_free (options);
}
