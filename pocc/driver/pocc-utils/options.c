/*
 * options.c: this file is part of the PoCC project.
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

#include <pocc-utils/options.h>


s_pocc_utils_options_t*
pocc_utils_options_malloc ()
{
  s_pocc_utils_options_t* ret = 
    (s_pocc_utils_options_t*) malloc (sizeof(s_pocc_utils_options_t));
  if (! ret)
    {
      fprintf (stderr, "[pocc-utils] Memory exhausted\n");
      exit (1);
    }

  ret->iterative = 0;
  ret->codegen_mode = POCC_UTILS_CODEGEN_FULL;

  return ret;
}


void
pocc_utils_options_free (s_pocc_utils_options_t* opts)
{
  if (opts)
    free (opts);
}
