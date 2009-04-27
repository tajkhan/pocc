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
#ifndef POCC_SRC_OPTIONS_H
# define POCC_SRC_OPTIONS_H

# include <stdio.h>

# if HAVE_CONFIG_H
#  include <pocc-utils/config.h>
# endif

# include <pocc/options.h>
# include "getopts.h"


# define POCC_NB_OPTS				35


# define POCC_OPT_HELP				0
# define POCC_OPT_VERBOSE			1
# define POCC_OPT_OUTFILE			2
# define POCC_OPT_COMPILE			3

# define POCC_OPT_LETSEE			4
# define POCC_OPT_LETSEE_SEARCHSPACE		5
# define POCC_OPT_LETSEE_TRAVERSAL		6
# define POCC_OPT_LETSEE_NORMSPACE		7
# define POCC_OPT_LETSEE_SCHEME_M1		8
# define POCC_OPT_LETSEE_RTRIES			9
# define POCC_OPT_LETSEE_PRUNE_PRECUT		10
# define POCC_OPT_LETSEE_BACKTRACK_MULTI	11

# define POCC_OPT_PLUTO				12
# define POCC_OPT_PLUTO_PARALLEL		13
# define POCC_OPT_PLUTO_TILE			14
# define POCC_OPT_PLUTO_L2TILE			15
# define POCC_OPT_PLUTO_FUSE			16
# define POCC_OPT_PLUTO_UNROLL			17
# define POCC_OPT_PLUTO_UFACTOR			18
# define POCC_OPT_PLUTO_POLYUNROLL		19
# define POCC_OPT_PLUTO_PREVECTOR		20
# define POCC_OPT_PLUTO_MULTIPIPE		21
# define POCC_OPT_PLUTO_RAR			22
# define POCC_OPT_PLUTO_LASTWRITER		23
# define POCC_OPT_PLUTO_SCALPRIV		24
# define POCC_OPT_PLUTO_BEE			25
# define POCC_OPT_PLUTO_QUIET			26
# define POCC_OPT_PLUTO_FT			27
# define POCC_OPT_PLUTO_LT			28

# define POCC_OPT_NOCODEGEN			29
# define POCC_OPT_CLOOG_F			30
# define POCC_OPT_CLOOG_L			31
# define POCC_OPT_CODEGEN_TIMERCODE		32
# define POCC_OPT_CODEGEN_TIMER_ASM		33
# define POCC_OPT_CODEGEN_TIMER_PAPI		34


BEGIN_C_DECLS

extern
int
pocc_getopts(s_pocc_options_t* options, int argc, char** argv);

END_C_DECLS

#endif // POCC_SRC_OPTIONS_H
