/*
 * driver-clan.c: this file is part of the PoCC project.
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

# include <pocc/driver-clan.h>


scoplib_scop_p
pocc_driver_clan (FILE* program,
		  s_pocc_options_t* poptions,
		  s_pocc_utils_options_t* puoptions)
{
  if (! poptions->quiet)
    printf ("[PoCC] Running Clan\n");
  clan_options_p coptions = clan_options_malloc ();
  coptions->bounded_context = poptions->clan_bounded_context;

  scoplib_scop_p scop = clan_scop_extract (program, coptions);

/*   scoplib_scop_print (stdout, scop); */

  return scop;
}
