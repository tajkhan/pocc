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


# define POCC_NB_OPTS				34


# define POCC_OPT_HELP				0
# define POCC_OPT_VERBOSE			1
# define POCC_OPT_OUTFILE			2

# define POCC_OPT_LETSEE			3
# define POCC_OPT_LETSEE_SEARCHSPACE		4
# define POCC_OPT_LETSEE_TRAVERSAL		5
# define POCC_OPT_LETSEE_NORMSPACE		6
# define POCC_OPT_LETSEE_SCHEME_M1		7
# define POCC_OPT_LETSEE_RTRIES			8
# define POCC_OPT_LETSEE_PRUNE_PRECUT		9
# define POCC_OPT_LETSEE_BACKTRACK_MULTI	10

# define POCC_OPT_PLUTO				11
# define POCC_OPT_PLUTO_PARALLEL		12
# define POCC_OPT_PLUTO_TILE			13
# define POCC_OPT_PLUTO_L2TILE			14
# define POCC_OPT_PLUTO_FUSE			15
# define POCC_OPT_PLUTO_UNROLL			16
# define POCC_OPT_PLUTO_UFACTOR			17
# define POCC_OPT_PLUTO_POLYUNROLL		18
# define POCC_OPT_PLUTO_PREVECTOR		19
# define POCC_OPT_PLUTO_MULTIPIPE		20
# define POCC_OPT_PLUTO_RAR			21
# define POCC_OPT_PLUTO_LASTWRITER		22
# define POCC_OPT_PLUTO_SCALPRIV		23
# define POCC_OPT_PLUTO_BEE			24
# define POCC_OPT_PLUTO_QUIET			25
# define POCC_OPT_PLUTO_FT			26
# define POCC_OPT_PLUTO_LT			27

# define POCC_OPT_NOCODEGEN			28
# define POCC_OPT_CLOOG_F			29
# define POCC_OPT_CLOOG_L			30
# define POCC_OPT_CODEGEN_TIMERCODE		31
# define POCC_OPT_CODEGEN_TIMER_ASM		32
# define POCC_OPT_CODEGEN_TIMER_PAPI		33


BEGIN_C_DECLS

extern
int
pocc_getopts(s_pocc_options_t* options, int argc, char** argv);

END_C_DECLS

#endif // POCC_SRC_OPTIONS_H
