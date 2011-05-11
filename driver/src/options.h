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


# define POCC_NB_OPTS				66


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
# define POCC_OPT_CANDL_DEP_ISL_SIMP		10

# define POCC_OPT_POLYFEAT			11
# define POCC_OPT_POLYFEAT_RAR			12

# define POCC_OPT_TRASH				13
# define POCC_OPT_VERBOSE			14
# define POCC_OPT_QUIET				15

# define POCC_OPT_LETSEE			16
# define POCC_OPT_LETSEE_SEARCHSPACE		17
# define POCC_OPT_LETSEE_TRAVERSAL		18
# define POCC_OPT_LETSEE_DRY_RUN          	19
# define POCC_OPT_LETSEE_NORMSPACE		20
# define POCC_OPT_LETSEE_BOUNDS			21
# define POCC_OPT_LETSEE_SCHEME_M1		22
# define POCC_OPT_LETSEE_RTRIES			23
# define POCC_OPT_LETSEE_PRUNE_PRECUT		24
# define POCC_OPT_LETSEE_BACKTRACK_MULTI	25

# define POCC_OPT_PLUTO				26
# define POCC_OPT_PLUTO_PARALLEL		27
# define POCC_OPT_PLUTO_TILE			28
# define POCC_OPT_PLUTO_L2TILE			29
# define POCC_OPT_PLUTO_FUSE			30
# define POCC_OPT_PLUTO_UNROLL			31
# define POCC_OPT_PLUTO_UFACTOR			32
# define POCC_OPT_PLUTO_POLYUNROLL		33
# define POCC_OPT_PLUTO_PREVECTOR		34
# define POCC_OPT_PLUTO_MULTIPIPE		35
# define POCC_OPT_PLUTO_RAR			36
# define POCC_OPT_PLUTO_RAR_CF			37
# define POCC_OPT_PLUTO_LASTWRITER		38
# define POCC_OPT_PLUTO_SCALPRIV		39
# define POCC_OPT_PLUTO_BEE			40
# define POCC_OPT_PLUTO_QUIET			41
# define POCC_OPT_PLUTO_FT			42
# define POCC_OPT_PLUTO_LT			43
# define POCC_OPT_PLUTO_EXTERNAL_CANDL	       	44
# define POCC_OPT_PLUTO_TILING_IN_SCATT	       	45
# define POCC_OPT_PLUTO_BOUND_COEF	       	46

# define POCC_OPT_NOCODEGEN			47
# define POCC_OPT_CLOOG_F			48
# define POCC_OPT_CLOOG_L			49
# define POCC_OPT_USE_PAST			50
# define POCC_OPT_PRAGMATIZER			51
# define POCC_OPT_VECTORIZER			52
# define POCC_OPT_VECT_MARK_PAR_LOOPS    	53
# define POCC_OPT_VECT_NO_KEEP_OUTER_PAR_LOOPS  54
# define POCC_OPT_VECT_SINK_ALL_LOOPS		55
# define POCC_OPT_STORCOMPACT			56
# define POCC_OPT_AC_KEEP_OUTERPAR		57
# define POCC_OPT_AC_KEEP_VECTORIZED		58
# define POCC_OPT_CODEGEN_TIMERCODE		59
# define POCC_OPT_CODEGEN_TIMER_ASM		60
# define POCC_OPT_CODEGEN_TIMER_PAPI		61
# define POCC_OPT_COMPILE      			62
# define POCC_OPT_COMPILE_CMD          		63
# define POCC_OPT_RUN_CMD_ARGS          	64
# define POCC_OPT_PROGRAM_TIMEOUT        	65


BEGIN_C_DECLS

extern
int
pocc_getopts(s_pocc_options_t* options, int argc, char** argv);

END_C_DECLS

#endif // POCC_SRC_OPTIONS_H
