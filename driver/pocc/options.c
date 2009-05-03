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
  ret->quiet = 0;
  ret->compile_command = NULL;
  ret->compile_program = 0;
  ret->execute_program = NULL;
  ret->program_exec_result = NULL;

  ret->cloog_options = NULL;

  // Letsee options.
  ret->letsee = 0; // Run LetSee (default: no)
  ret->letsee_space = LS_TYPE_FS;
  ret->letsee_traversal = LS_HEURISTIC_EXHAUST;
  ret->letsee_normspace = 0;
  ret->letsee_scheme_m1 = NULL;
  ret->letsee_prune_precut = 0;
  ret->letsee_backtrack_multi = 0;
  ret->letsee_rtries = 50;
  ret->letsee_ilb = -1;
  ret->letsee_iUb = 1;
  ret->letsee_plb = -1;
  ret->letsee_pUb = 1;
  ret->letsee_clb = -1;
  ret->letsee_cUb = 1;

  // Pluto options.
  ret->pluto_parallel = 0;
  ret->pluto_tile = 0;
  ret->pluto_rar = 0;
  ret->pluto_fuse = PLUTO_SMART_FUSE;
  ret->pluto_unroll = 0;
  ret->pluto_polyunroll = 0;
  ret->pluto_bee = 0;
  ret->pluto_prevector = 0;
  ret->pluto_ufactor = 4;
  ret->pluto_quiet = 0;
  ret->pluto_context = 1;
  ret->pluto_ft = -1;
  ret->pluto_lt = -1;
  ret->pluto_multipipe = 0;
  ret->pluto_l2tile = 0;
  ret->pluto_lastwriter = 0;
  ret->pluto_scalpriv = 0;

  // Cloog options.
  ret->cloog_f = POCC_CLOOG_UNDEF;
  ret->cloog_l = POCC_CLOOG_UNDEF;

  // Codegen options.
  ret->codegen = 1;
  ret->codegen_timercode = 0;
  ret->codegen_timer_asm = 0;
  ret->codegen_timer_papi = 0;

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
  XFREE(options->input_file_name);
  if (options->output_file_name)
    XFREE(options->output_file_name);
  if (options->cloog_options)
    cloog_options_free (options->cloog_options);
  if (options->letsee_scheme_m1)
    XFREE(options->letsee_scheme_m1);
  if (options->compile_command)
    XFREE(options->compile_command);
  if (options->program_exec_result)
    XFREE(options->program_exec_result);

  XFREE(options);
}
