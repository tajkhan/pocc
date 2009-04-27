/*
 * driver-pluto.c: this file is part of the PoCC project.
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

# include <pocc/driver-pluto.h>


void 
pocc_driver_pluto (clan_scop_p program, 
		  s_pocc_options_t* poptions,
		  s_pocc_utils_options_t* puoptions)
{
  printf ("[PoCC] Running Pluto...\n");
  PlutoOptions* ploptions = pluto_options_alloc ();
  pocc_options_init_cloog (poptions);
  puoptions->cloog_options = (void*) poptions->cloog_options;
  printf("HERE\n");
  pluto_pocc (program, ploptions, puoptions);
  clan_scop_print (stdout, program);
}
