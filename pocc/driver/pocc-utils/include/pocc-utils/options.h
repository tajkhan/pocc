/*
 * options.h: this file is part of the PoCC project.
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
#ifndef POCC_UTILS_OPTIONS_H
# define POCC_UTILS_OPTIONS_H

# include <stdio.h>
# include <stdlib.h>

# include <pocc-utils/types.h>


# define POCC_UTILS_CODEGEN_FULL	0
# define POCC_UTILS_CODEGEN_PLUTO	1


# ifdef __cplusplus
#  define BEGIN_C_DECLS         extern "C" {
#  define END_C_DECLS           }
# else
#  define BEGIN_C_DECLS
#  define END_C_DECLS
# endif

BEGIN_C_DECLS

struct s_pocc_utils_options;
typedef void (*pocc_codegen_fun_t)(struct s_pocc_utils_options*);

struct s_pocc_utils_options
{
  int			codegen_mode;
  int			iterative;
  void*			cloog_options; // CloogOptions*
  void*			pocc_options; // s_pocc_options_t*
  void*			program; // clan_scop_p
  pocc_codegen_fun_t	pocc_codegen;
  char*			program_exec_result;
  char*			input_file_name;
  char*			output_file_name;
  FILE*			data_file;
};
typedef struct s_pocc_utils_options s_pocc_utils_options_t;

extern
s_pocc_utils_options_t* pocc_utils_options_malloc ();

extern
void pocc_utils_options_free (s_pocc_utils_options_t* opts);

END_C_DECLS


#endif // POCC_UTILS_OPTIONS_H
