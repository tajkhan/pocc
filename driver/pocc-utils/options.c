/*
 * options.c: this file is part of the PoCC project.
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
  ret->point_idx = 0;
  ret->transfo_matrices = NULL;
  ret->codegen_mode = POCC_UTILS_CODEGEN_FULL;
  ret->cloog_options = NULL;
  ret->pocc_options = NULL;

  ret->program; // clan_scop_p
  ret->pocc_codegen = NULL;
  ret->program_exec_result = NULL;

  ret->input_file_name = NULL;
  ret->output_file_name = NULL;

  ret->data_file = NULL;

  return ret;
}


void
pocc_utils_options_free (s_pocc_utils_options_t* opts)
{
  if (opts)
    free (opts);
}
