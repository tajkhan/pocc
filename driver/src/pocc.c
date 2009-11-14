/*
 * pocc.c: this file is part of the PoCC project.
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
#include <pocc/common.h>

#include <stdio.h>
#include "getopts.h"
#include "options.h"
#include <pocc/driver-clan.h>
#include <pocc/driver-candl.h>
#include <pocc/driver-letsee.h>
#include <pocc/driver-pluto.h>
#include <pocc/driver-codegen.h>
#include <pocc/driver-clastops.h>
#include <pocc/exec.h>



int main(int argc, char** argv)
{
  // (0) Initialize and get options.
  s_pocc_options_t* poptions = pocc_options_malloc ();
  s_pocc_utils_options_t* puoptions = pocc_utils_options_malloc ();
  pocc_getopts (poptions, argc, argv);

  if (! poptions->quiet)
    printf ("[PoCC] Compiling file: %s\n", poptions->input_file_name);
  if (poptions->trash)
    {
      if (! poptions->quiet)
	printf ("[PoCC] Deleting files previously generated by PoCC for %s\n",
		poptions->input_file_name);
      char* args[3];
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/trash";
      args[1] = strdup (poptions->input_file_name);
      args[1][strlen (args[1]) - 2] = '\0';
      args[2] = NULL;
      pocc_exec (args, POCC_EXECV_HIDE_OUTPUT);
      XFREE(args[1]);
    }
  if (poptions->letsee == 0 && poptions->pluto == 0)
    if (! poptions->quiet)
      printf ("[PoCC] INFO: pass-thru compilation, no optimization enabled\n");

  // (1) Parse the file.
  scoplib_scop_p scop =
    pocc_driver_clan (poptions->input_file, poptions, puoptions);
  if (! scop || scop->statement == NULL)
    pocc_error ("[PoCC] Possible parsing error: no statement in SCoP");
  // (2) If pass-thru, run candl.
  if (! poptions->letsee || ! poptions->pluto)
    pocc_driver_candl (scop, poptions, puoptions);

  // (3) Perform LetSee.
  if (poptions->letsee)
    pocc_driver_letsee (scop, poptions, puoptions);

  // (4) Perform PLuTo.
  // Don't do it if already performed through LetSee.
  if (poptions->pluto && ! poptions->letsee)
    if (pocc_driver_pluto (scop, poptions, puoptions) == EXIT_FAILURE)
      exit (EXIT_FAILURE);

  // (5) Perform codgen.
  // Don't do it if already performed through LetSee.
  if (poptions->codegen && ! poptions->letsee)
    pocc_driver_codegen (scop, poptions, puoptions);

  // Be clean.
  scoplib_scop_free (puoptions->program);
  pip_close ();
  if (! poptions->quiet)
    printf ("[PoCC] All done.\n");
  pocc_options_free (poptions);

  return 0;
}
