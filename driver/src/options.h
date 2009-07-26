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


# define POCC_NB_OPTS				43


# define POCC_OPT_HELP				0
# define POCC_OPT_VERSION			1
# define POCC_OPT_OUTFILE			2
# define POCC_OPT_BOUNDED_CTXT	       		3

# define POCC_OPT_TRASH				4
# define POCC_OPT_VERBOSE			5
# define POCC_OPT_QUIET				6

# define POCC_OPT_LETSEE			7
# define POCC_OPT_LETSEE_SEARCHSPACE		8
# define POCC_OPT_LETSEE_TRAVERSAL		9
# define POCC_OPT_LETSEE_DRY_RUN          	10
# define POCC_OPT_LETSEE_NORMSPACE		11
# define POCC_OPT_LETSEE_BOUNDS			12
# define POCC_OPT_LETSEE_SCHEME_M1		13
# define POCC_OPT_LETSEE_RTRIES			14
# define POCC_OPT_LETSEE_PRUNE_PRECUT		15
# define POCC_OPT_LETSEE_BACKTRACK_MULTI	16

# define POCC_OPT_PLUTO				17
# define POCC_OPT_PLUTO_PARALLEL		18
# define POCC_OPT_PLUTO_TILE			19
# define POCC_OPT_PLUTO_L2TILE			20
# define POCC_OPT_PLUTO_FUSE			21
# define POCC_OPT_PLUTO_UNROLL			22
# define POCC_OPT_PLUTO_UFACTOR			23
# define POCC_OPT_PLUTO_POLYUNROLL		24
# define POCC_OPT_PLUTO_PREVECTOR		25
# define POCC_OPT_PLUTO_MULTIPIPE		26
# define POCC_OPT_PLUTO_RAR			27
# define POCC_OPT_PLUTO_LASTWRITER		28
# define POCC_OPT_PLUTO_SCALPRIV		29
# define POCC_OPT_PLUTO_BEE			30
# define POCC_OPT_PLUTO_QUIET			31
# define POCC_OPT_PLUTO_FT			32
# define POCC_OPT_PLUTO_LT			33

# define POCC_OPT_NOCODEGEN			34
# define POCC_OPT_CLOOG_F			35
# define POCC_OPT_CLOOG_L			36
# define POCC_OPT_CODEGEN_TIMERCODE		37
# define POCC_OPT_CODEGEN_TIMER_ASM		38
# define POCC_OPT_CODEGEN_TIMER_PAPI		39

# define POCC_OPT_COMPILE      			40
# define POCC_OPT_COMPILE_CMD          		41
# define POCC_OPT_PROGRAM_TIMEOUT        	42


BEGIN_C_DECLS

extern
int
pocc_getopts(s_pocc_options_t* options, int argc, char** argv);

END_C_DECLS

#endif // POCC_SRC_OPTIONS_H
