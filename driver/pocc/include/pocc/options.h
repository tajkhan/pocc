/*
 * options.h: this file is part of the PoCC project.
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
#ifndef POCC_OPTIONS_H
# define POCC_OPTIONS_H

# include <stdio.h>
# include <pocc/common.h>
# include <cloog/options.h>
# include <pluto/pluto.h>
# include <letsee/options.h>

#define POCC_TIMER_CODE_ASM	1
#define POCC_TIMER_CODE_TIME	2

#define POCC_CLOOG_UNDEF	-2

struct s_pocc_options
{
  // In/Out file information
  FILE*		input_file;
  char*		input_file_name;
  FILE*		output_file;
  char*		output_file_name;

  // Verbose.
  int		verbose;
  int		quiet;

  // Trash.
  int		trash;

  // Parser options.
  int		clan_bounded_context;
  int		inscop_fakepoccarray;

  // Compile command.
  int		compile_program; // Internal field
  char*		compile_command;
  int		execute_program;
  char*		execute_command_args;
  char*		program_exec_result; // Internal field

  // LetSee Options.
  int		letsee; // Run LetSee (default: no)
  int		letsee_space;
  int		letsee_traversal;
  int		letsee_normspace;
  int*		letsee_scheme_m1;
  int		letsee_prune_precut;
  int		letsee_backtrack_multi;
  int		letsee_rtries;
  int		letsee_ilb;
  int		letsee_iUb;
  int		letsee_plb;
  int		letsee_pUb;
  int		letsee_clb;
  int		letsee_cUb;

  // PLuTo Options.
  int		pluto; // Run PLuTo (default: no)
  int		pluto_unroll;
  int		pluto_parallel;
  int		pluto_tile;
  int		pluto_rar;
  int		pluto_fuse;
  int		pluto_polyunroll;
  int		pluto_bee;
  int		pluto_prevector;
  int		pluto_ufactor;
  int		pluto_quiet;
  int		pluto_context;
  int		pluto_ft;
  int		pluto_lt;
  int		pluto_multipipe;
  int		pluto_l2tile;
  int		pluto_lastwriter;
  int		pluto_scalpriv;


  // Codegen Options.
  int		codegen; // Perform codegen (default: yes)
  CloogOptions*	cloog_options;
  int		cloog_f;
  int		cloog_l;
  int		codegen_timercode;
  int		codegen_timer_asm;
  int		codegen_timer_papi;
  int		timeout;

};
typedef struct s_pocc_options s_pocc_options_t;



BEGIN_C_DECLS

extern
s_pocc_options_t* pocc_options_malloc ();

extern
void pocc_options_init_cloog (s_pocc_options_t* options);

extern
void pocc_options_free (s_pocc_options_t* options);


END_C_DECLS



#endif // POCC_OPTIONS_H
