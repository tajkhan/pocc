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


# define POCC_NB_OPTS				42


# define POCC_OPT_HELP				0
# define POCC_OPT_VERSION			1
# define POCC_OPT_OUTFILE			2
# define POCC_OPT_TRASH				3
# define POCC_OPT_VERBOSE			4
# define POCC_OPT_QUIET				5

# define POCC_OPT_LETSEE			6
# define POCC_OPT_LETSEE_SEARCHSPACE		7
# define POCC_OPT_LETSEE_TRAVERSAL		8
# define POCC_OPT_LETSEE_DRY_RUN          	9
# define POCC_OPT_LETSEE_NORMSPACE		10
# define POCC_OPT_LETSEE_BOUNDS			11
# define POCC_OPT_LETSEE_SCHEME_M1		12
# define POCC_OPT_LETSEE_RTRIES			13
# define POCC_OPT_LETSEE_PRUNE_PRECUT		14
# define POCC_OPT_LETSEE_BACKTRACK_MULTI	15

# define POCC_OPT_PLUTO				16
# define POCC_OPT_PLUTO_PARALLEL		17
# define POCC_OPT_PLUTO_TILE			18
# define POCC_OPT_PLUTO_L2TILE			19
# define POCC_OPT_PLUTO_FUSE			20
# define POCC_OPT_PLUTO_UNROLL			21
# define POCC_OPT_PLUTO_UFACTOR			22
# define POCC_OPT_PLUTO_POLYUNROLL		23
# define POCC_OPT_PLUTO_PREVECTOR		24
# define POCC_OPT_PLUTO_MULTIPIPE		25
# define POCC_OPT_PLUTO_RAR			26
# define POCC_OPT_PLUTO_LASTWRITER		27
# define POCC_OPT_PLUTO_SCALPRIV		28
# define POCC_OPT_PLUTO_BEE			29
# define POCC_OPT_PLUTO_QUIET			30
# define POCC_OPT_PLUTO_FT			31
# define POCC_OPT_PLUTO_LT			32

# define POCC_OPT_NOCODEGEN			33
# define POCC_OPT_CLOOG_F			34
# define POCC_OPT_CLOOG_L			35
# define POCC_OPT_CODEGEN_TIMERCODE		36
# define POCC_OPT_CODEGEN_TIMER_ASM		37
# define POCC_OPT_CODEGEN_TIMER_PAPI		38

# define POCC_OPT_COMPILE      			39
# define POCC_OPT_COMPILE_CMD          		40
# define POCC_OPT_PROGRAM_TIMEOUT        	41


BEGIN_C_DECLS

extern
int
pocc_getopts(s_pocc_options_t* options, int argc, char** argv);

END_C_DECLS

#endif // POCC_SRC_OPTIONS_H
