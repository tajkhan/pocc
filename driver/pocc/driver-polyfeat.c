/*
 * driver-polyfeat.c: this file is part of the PoCC project.
 *
 * PoCC, the Polyhedral Compiler Collection package
 *
 * Copyright (C) 2011 Louis-Noel Pouchet
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

# include <pocc/driver-polyfeat.h>


void
pocc_driver_polyfeat (scoplib_scop_p program,
		      s_pocc_options_t* poptions,
		      s_pocc_utils_options_t* puoptions)
{
  if (! poptions->quiet)
    printf ("[PoCC] Running Polyfeat\n");
  char buffer[strlen (poptions->output_file_name) + 6];
  strcpy (buffer, poptions->output_file_name);
  strcat (buffer, ".feat");
  FILE* outfile = fopen (buffer, "w");
  if (outfile)
    {
      polyfeat_depgraph_extract (outfile, program, poptions->polyfeat_rar);
      fclose (outfile);
    }
  else
    pocc_error ("Cannot open output file\n");
}
