/*
 * exec.h: this file is part of the PoCC project.
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
#ifndef POCC_UTILS_H
# define POCC_UTILS_H

# include <stdio.h>

# if HAVE_CONFIG_H
#  include <pocc-utils/config.h>
# endif

#include <pocc/common.h>

# define TO_STRING__(x) #x
# define TO_STRING_(x) TO_STRING__(x)
# define STR_POCC_ROOT_DIR TO_STRING_(POCC_ROOT_DIR)

# define POCC_EXECV_HIDE_OUTPUT 0
# define POCC_EXECV_SHOW_OUTPUT 1
# define POCC_EXECV_NOEXIT	2


BEGIN_C_DECLS

extern void
pocc_exec (char** args, int show_output);

extern char*
pocc_exec_string (char** args, int show_output);

extern char*
pocc_exec_string_noexit (char** args, int show_output);

END_C_DECLS


#endif // POCC_ERROR_H
