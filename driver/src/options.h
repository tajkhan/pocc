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
#ifndef POCC_SRC_OPTIONS_H
# define POCC_SRC_OPTIONS_H

# include <stdio.h>

# if HAVE_CONFIG_H
#  include <pocc-utils/config.h>
# endif

# include <pocc/options.h>
# include "getopts.h"


# define POCC_NB_OPTS				61


# define POCC_OPT_HELP				0
# define POCC_OPT_VERSION			1
# define POCC_OPT_OUTFILE			2
# define POCC_OPT_OUTFILE_SCOP			3
# define POCC_OPT_CLOOGIFY_SCHED       		4
# define POCC_OPT_BOUNDED_CTXT	       		5
# define POCC_OPT_DEFAULT_CTXT	       		6
# define POCC_OPT_INSCOP_FAKEARRAY		7
# define POCC_OPT_READ_SCOP_FILE		8

# define POCC_OPT_NO_CANDL			9

# define POCC_OPT_TRASH				10
# define POCC_OPT_VERBOSE			11
# define POCC_OPT_QUIET				12

# define POCC_OPT_LETSEE			13
# define POCC_OPT_LETSEE_SEARCHSPACE		14
# define POCC_OPT_LETSEE_TRAVERSAL		15
# define POCC_OPT_LETSEE_DRY_RUN          	16
# define POCC_OPT_LETSEE_NORMSPACE		17
# define POCC_OPT_LETSEE_BOUNDS			18
# define POCC_OPT_LETSEE_SCHEME_M1		19
# define POCC_OPT_LETSEE_RTRIES			20
# define POCC_OPT_LETSEE_PRUNE_PRECUT		21
# define POCC_OPT_LETSEE_BACKTRACK_MULTI	22

# define POCC_OPT_PLUTO				23
# define POCC_OPT_PLUTO_PARALLEL		24
# define POCC_OPT_PLUTO_TILE			25
# define POCC_OPT_PLUTO_L2TILE			26
# define POCC_OPT_PLUTO_FUSE			27
# define POCC_OPT_PLUTO_UNROLL			28
# define POCC_OPT_PLUTO_UFACTOR			29
# define POCC_OPT_PLUTO_POLYUNROLL		30
# define POCC_OPT_PLUTO_PREVECTOR		31
# define POCC_OPT_PLUTO_MULTIPIPE		32
# define POCC_OPT_PLUTO_RAR			33
# define POCC_OPT_PLUTO_RAR_CF			34
# define POCC_OPT_PLUTO_LASTWRITER		35
# define POCC_OPT_PLUTO_SCALPRIV		36
# define POCC_OPT_PLUTO_BEE			37
# define POCC_OPT_PLUTO_QUIET			38
# define POCC_OPT_PLUTO_FT			39
# define POCC_OPT_PLUTO_LT			40
# define POCC_OPT_PLUTO_EXTERNAL_CANDL	       	41
# define POCC_OPT_PLUTO_TILING_IN_SCATT	       	42

# define POCC_OPT_NOCODEGEN			43
# define POCC_OPT_CLOOG_F			44
# define POCC_OPT_CLOOG_L			45
# define POCC_OPT_PRAGMATIZER			46
# define POCC_OPT_VECTORIZER			47
# define POCC_OPT_VECT_MARK_PAR_LOOPS    	48
# define POCC_OPT_VECT_NO_KEEP_OUTER_PAR_LOOPS  49
# define POCC_OPT_VECT_SINK_ALL_LOOPS		50
# define POCC_OPT_STORCOMPACT			51
# define POCC_OPT_AC_KEEP_OUTERPAR		52
# define POCC_OPT_AC_KEEP_VECTORIZED		53
# define POCC_OPT_CODEGEN_TIMERCODE		54
# define POCC_OPT_CODEGEN_TIMER_ASM		55
# define POCC_OPT_CODEGEN_TIMER_PAPI		56
# define POCC_OPT_COMPILE      			57
# define POCC_OPT_COMPILE_CMD          		58
# define POCC_OPT_RUN_CMD_ARGS          	59
# define POCC_OPT_PROGRAM_TIMEOUT        	60


BEGIN_C_DECLS

extern
int
pocc_getopts(s_pocc_options_t* options, int argc, char** argv);

END_C_DECLS

#endif // POCC_SRC_OPTIONS_H
