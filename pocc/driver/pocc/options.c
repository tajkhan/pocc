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

# include <pocc/options.h>


s_pocc_options_t*
pocc_options_malloc ()
{
  s_pocc_options_t* ret = XMALLOC(s_pocc_options_t, 1);
  ret->input_file = NULL;
  ret->input_file_name = NULL;
  ret->output_file = NULL;
  ret->output_file_name = NULL;
  ret->verbose = 0;
  ret->cloog_options = NULL;

  ret->vectorize = 0;
  ret->parallel = 0;
  ret->unroll = 0;
  ret->timer = 0;

  return ret;
}


void
pocc_options_init_cloog (s_pocc_options_t* options)
{
  options->cloog_options = cloog_options_malloc ();
/*   options->cloog_codegen = (void*) cloog_program_generate; */
}

void
pocc_options_free (s_pocc_options_t* options)
{
  fclose (options->input_file);
  XFREE(options->input_file_name);
  if (options->output_file)
    fclose (options->output_file);
  if (options->output_file_name)
    XFREE(options->output_file_name);
  if (options->cloog_options)
    cloog_options_free (options->cloog_options);

  XFREE(options);
}
