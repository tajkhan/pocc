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
#if HAVE_CONFIG_H
# include <pocc-utils/config.h>
#endif

#include <stdio.h>
#include "getopts.h"
#include "options.h"
#include <pocc/driver-clan.h>
#include <pocc/driver-candl.h>
#include <pocc/driver-letsee.h>
#include <pocc/driver-pluto.h>
#include <pocc/driver-codegen.h>
#include <pocc/driver-cloog.h>



int main(int argc, char** argv)
{
  // (0) Initialize and get options.
  s_pocc_options_t* poptions = pocc_options_malloc ();
  s_pocc_utils_options_t* puoptions = pocc_utils_options_malloc ();
  pocc_getopts (poptions, argc, argv);

  printf ("[PoCC] Compiling file: %s\n", poptions->input_file_name);
  if (poptions->letsee == 0 && poptions->pluto == 0)
    printf ("[PoCC] INFO: pass-thru compilation, no optimization enabled\n");

  // (1) Parse the file.
  clan_scop_p scop =
    pocc_driver_clan (poptions->input_file, poptions, puoptions);
  if (! scop || scop->statement == NULL)
    pocc_error ("Possible parsing error: no statement in SCoP");

  // (2) Perform LetSee.
  if (poptions->letsee)
    {
      pocc_driver_letsee (scop, poptions, puoptions);
    }

  // (3) Perform PLuTo.
  // Don't do it if already performed through LetSee.
  if (poptions->pluto && ! poptions->letsee)
    {
      pocc_driver_pluto (scop, poptions, puoptions);
    }

  // (3) Perform codgen.
  // Don't do it if already performed through LetSee.
  if (poptions->codegen && ! poptions->letsee)
    {
      pocc_driver_codegen (scop, poptions, puoptions);
    }

  // Be clean.
  clan_scop_free (puoptions->program);
  pip_close ();
  if (! poptions->letsee)
    printf ("[PoCC] Output is %s. All done.\n", poptions->output_file_name);
  pocc_options_free (poptions);

  return 0;
}