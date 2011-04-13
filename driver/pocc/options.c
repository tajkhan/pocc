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

# include <pocc/options.h>


s_pocc_options_t*
pocc_options_malloc ()
{
  s_pocc_options_t* ret = XMALLOC(s_pocc_options_t, 1);
  ret->input_file = NULL;
  ret->input_file_name = NULL;
  ret->output_file = NULL;
  ret->output_file_name = NULL;
  ret->output_scoplib_file_name = NULL;
  ret->names_are_strings = 1;
  ret->trash = 0;
  ret->verbose = 0;
  ret->quiet = 0;
  ret->compile_command = NULL;
  ret->compile_program = 0;
  ret->execute_command_args = NULL;
  ret->execute_program = 0;
  ret->program_exec_result = NULL;
  ret->clan_bounded_context = 0;
  ret->inscop_fakepoccarray = 0;
  ret->set_default_parameter_values = 0;
  ret->read_input_scop_file = 0;
  ret->cloog_options = NULL;
  ret->cloogify_schedules = 0;

  // Dependence analysis.
  ret->candl_pass = 1;
  ret->candl_deps_isl_simplify = 0;

  // Feature extraction.
  ret->polyfeat = 0;

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
  ret->pluto = 0;
  ret->pluto_parallel = 0;
  ret->pluto_tile = 0;
  ret->pluto_rar = 0;
  ret->pluto_fuse = SMART_FUSE;
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
  ret->pluto_external_candl = 0;
  ret->pluto_rar_cf = 0;
  ret->pluto_tiling_in_scatt = 0;
  ret->pluto_bound_coefficients = 0;

  // Cloog options.
  ret->cloog_f = POCC_CLOOG_UNDEF;
  ret->cloog_l = POCC_CLOOG_UNDEF;

  // Codegen options.
  ret->codegen = 1;
  ret->codegen_timercode = 0;
  ret->codegen_timer_asm = 0;
  ret->codegen_timer_papi = 0;
  ret->timeout = 0;

  ret->pragmatizer = 0;

  // Vectorization options.
  ret->vectorizer = 0;
  ret->vectorizer_mark_par_loops = 0;
  ret->vectorizer_mark_vect_loops = 1;
  ret->vectorizer_vectorize_loops = 1;
  ret->vectorizer_keep_outer_par_loops = 1;
  ret->vectorizer_sink_all_candidates = 0;

  // Storage compaction options.
  ret->storage_compaction = 0;
  ret->array_contraction_keep_outer_par_loops = 0;
  ret->array_contraction_keep_vectorized_loops = 0;

  // CLAST annotation/translation.
  ret->clastannotation_pass = 0;

  return ret;
}


void
pocc_options_init_cloog (s_pocc_options_t* options)
{
  CloogState* cstate = cloog_state_malloc ();
  options->cloog_options = cloog_options_malloc (cstate);
/*   options->cloog_codegen = (void*) cloog_program_generate; */
}


void
pocc_options_free (s_pocc_options_t* options)
{
  if (options->input_file_name)
    XFREE(options->input_file_name);
  if (options->output_file_name)
    XFREE(options->output_file_name);
  if (options->output_scoplib_file_name)
    XFREE(options->output_scoplib_file_name);
  if (options->cloog_options)
    {
      CloogState* cstate = options->cloog_options->state;
      cloog_options_free (options->cloog_options);
      cloog_state_free (cstate);
    }
  if (options->letsee_scheme_m1)
    XFREE(options->letsee_scheme_m1);
  if (options->compile_command)
    XFREE(options->compile_command);
  if (options->execute_command_args)
    XFREE(options->execute_command_args);
  if (options->program_exec_result)
    XFREE(options->program_exec_result);

  XFREE(options);
}
