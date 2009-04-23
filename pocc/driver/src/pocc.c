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

  pocc_driver_clan (NULL, NULL, NULL);
  pocc_driver_candl (NULL, NULL, NULL);
  pocc_driver_letsee (NULL, NULL, NULL);
  pocc_driver_pluto (NULL, NULL, NULL);
  pocc_driver_codegen (NULL, NULL, NULL);
  pocc_driver_cloog (NULL, NULL, NULL);

  pip_close ();

  printf ("PoCC compiler: done\n");

  return 0;
}
