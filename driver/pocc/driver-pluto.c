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


int
pocc_driver_pluto (scoplib_scop_p program, 
		  s_pocc_options_t* poptions,
		  s_pocc_utils_options_t* puoptions)
{
  printf ("[PoCC] Running Pluto\n");

  PlutoOptions* ploptions = pluto_options_alloc ();
  ploptions->parallel = poptions->pluto_parallel;
  ploptions->tile = poptions->pluto_tile;
  ploptions->rar = poptions->pluto_rar;
  ploptions->unroll = poptions->pluto_unroll;
  ploptions->fuse = poptions->pluto_fuse;
  ploptions->polyunroll = poptions->pluto_polyunroll;
  ploptions->bee = poptions->pluto_bee;
  ploptions->prevector = poptions->pluto_prevector;
  ploptions->ufactor = poptions->pluto_ufactor; // int
  ploptions->quiet = poptions->pluto_quiet;
  ploptions->context = poptions->pluto_context; // int
  ploptions->cloogf = poptions->cloog_f; // int
  ploptions->cloogl = poptions->cloog_l; // int
  ploptions->ft = poptions->pluto_ft; // int
  ploptions->lt = poptions->pluto_lt; // int
  ploptions->multipipe = poptions->pluto_multipipe;
  ploptions->l2tile = poptions->pluto_l2tile;
  ploptions->lastwriter = poptions->pluto_lastwriter;
  ploptions->scalpriv = poptions->pluto_scalpriv;
  ploptions->debug = poptions->verbose;
  
  pocc_options_init_cloog (poptions);
  puoptions->cloog_options = (void*) poptions->cloog_options;
  if (pluto_pocc (program, ploptions, puoptions) == PLUTO_EXIT_ERROR)
    return EXIT_FAILURE;
  poptions->cloog_options = puoptions->cloog_options;
  
/*   scoplib_scop_print (stdout, program); */
  return EXIT_SUCCESS;
}
