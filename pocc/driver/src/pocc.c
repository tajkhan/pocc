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


int pass_thru(s_pocc_options_t* options)
{

}


int main(int argc, char** argv)
{
  printf ("PoCC compiler\n");
  FILE* f = fopen (argv[1], "r");
  if (!f)
    {
      fprintf (stderr, "Cannot open file: %s\n", argv[1]);
      exit (1);
    }

  s_pocc_options_t* poptions = pocc_options_malloc ();
  s_pocc_utils_options_t* puoptions = pocc_utils_options_malloc ();

  poptions->input_file_name = argv[1];
  poptions->output_file_name = "outputpocc.c";

  // (1) Parse the file.
  clan_scop_p scop = pocc_driver_clan (f, poptions, puoptions);

  if (! scop || scop->statement == NULL)
    pocc_error ("Possible parsing error: no statement in SCoP");

  // (2) Perform LetSee.
  pocc_driver_letsee (scop, poptions, puoptions);

  // (3) Perform PLuTo.
  pocc_driver_pluto (scop, poptions, puoptions);

  // (3) Perform codgen.
  pocc_driver_codegen (scop, poptions, puoptions);


  fclose (f);
  pip_close ();

  printf ("PoCC compiler: done\n");

  return 0;
}