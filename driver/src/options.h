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

# ifndef POCC_RELEASE_MODE
#  define POCC_NB_OPTS				86
# else
#  define POCC_NB_OPTS				68
# endif

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
# define POCC_OPT_CANDL_DEP_PRUNE_DUPS		11

# define POCC_OPT_POLYFEAT			12
# define POCC_OPT_POLYFEAT_RAR			13

# define POCC_OPT_TRASH				14
# define POCC_OPT_VERBOSE			15
# define POCC_OPT_QUIET				16

# define POCC_OPT_LETSEE			17
# define POCC_OPT_LETSEE_SEARCHSPACE		18
# define POCC_OPT_LETSEE_TRAVERSAL		19
# define POCC_OPT_LETSEE_DRY_RUN          	20
# define POCC_OPT_LETSEE_NORMSPACE		21
# define POCC_OPT_LETSEE_BOUNDS			22
# define POCC_OPT_LETSEE_SCHEME_M1		23
# define POCC_OPT_LETSEE_RTRIES			24
# define POCC_OPT_LETSEE_PRUNE_PRECUT		25
# define POCC_OPT_LETSEE_BACKTRACK_MULTI	26

# define POCC_OPT_PLUTO				27
# define POCC_OPT_PLUTO_PARALLEL		28
# define POCC_OPT_PLUTO_TILE			29
# define POCC_OPT_PLUTO_L2TILE			30
# define POCC_OPT_PLUTO_FUSE			31
# define POCC_OPT_PLUTO_UNROLL			32
# define POCC_OPT_PLUTO_UFACTOR			33
# define POCC_OPT_PLUTO_POLYUNROLL		34
# define POCC_OPT_PLUTO_PREVECTOR		35
# define POCC_OPT_PLUTO_MULTIPIPE		36
# define POCC_OPT_PLUTO_RAR			37
# define POCC_OPT_PLUTO_RAR_CF			38
# define POCC_OPT_PLUTO_LASTWRITER		39
# define POCC_OPT_PLUTO_SCALPRIV		40
# define POCC_OPT_PLUTO_BEE			41
# define POCC_OPT_PLUTO_QUIET			42
# define POCC_OPT_PLUTO_FT			43
# define POCC_OPT_PLUTO_LT			44
# define POCC_OPT_PLUTO_EXTERNAL_CANDL	       	45
# define POCC_OPT_PLUTO_TILING_IN_SCATT	       	46
# define POCC_OPT_PLUTO_BOUND_COEF	       	47

# define POCC_OPT_NOCODEGEN			48
# define POCC_OPT_CLOOG_F			49
# define POCC_OPT_CLOOG_L			50
# define POCC_OPT_PRINT_CLOOG_FILE		51
# define POCC_OPT_NO_PAST			52
# define POCC_OPT_PAST_OPTIMIZE_LOOP_BOUND	53
# define POCC_OPT_PRAGMATIZER			54

# define POCC_OPT_PTILE				55
# define POCC_OPT_PTILE_FTS    			56

# define POCC_OPT_PUNROLL			57
# define POCC_OPT_PUNROLL_AND_JAM      		58
# define POCC_OPT_PUNROLL_SIZE      		59

# define POCC_OPT_VECTORIZER			60

/* Devel-only options. */
# define POCC_OPT_VECT_MARK_PAR_LOOPS    	61
# define POCC_OPT_VECT_NO_KEEP_OUTER_PAR_LOOPS  62
# define POCC_OPT_VECT_SINK_ALL_LOOPS		63
# define POCC_OPT_STORCOMPACT			64
# define POCC_OPT_AC_KEEP_OUTERPAR		65
# define POCC_OPT_AC_KEEP_VECTORIZED		66

# ifndef POCC_RELEASE_MODE
#  define POCC_OPT_CODEGEN_TIMERCODE		67
#  define POCC_OPT_CODEGEN_TIMER_ASM		68
#  define POCC_OPT_CODEGEN_TIMER_PAPI		69
#  define POCC_OPT_COMPILE      		70
#  define POCC_OPT_COMPILE_CMD          	71
#  define POCC_OPT_RUN_CMD_ARGS          	72
#  define POCC_OPT_PROGRAM_TIMEOUT        	73

#  define POCC_OPT_PONOS	        	74
#  define POCC_OPT_PONOS_QUIET	        	75
#  define POCC_OPT_PONOS_DEBUG	        	76
#  define POCC_OPT_PONOS_SCHED_DIMENSION	77
#  define POCC_OPT_PONOS_COEF_ARE_POS	        78
#  define POCC_OPT_PONOS_BUILD_2DP1	        79
#  define POCC_OPT_PONOS_SOLVER_TYPE	        80
#  define POCC_OPT_PONOS_MAXSCALE_SOLVER	81
#  define POCC_OPT_PONOS_NOREDUNDANCY_SOLVER	82
#  define POCC_OPT_PONOS_LEGALITY_CONSTANT_K	83
#  define POCC_OPT_PONOS_SCHED_COEF_BOUND	84

#  define POCC_OPT_PAST_SUPER_OPT_LOOP_BOUND	85


# else
#  define POCC_OPT_CODEGEN_TIMERCODE		61
#  define POCC_OPT_CODEGEN_TIMER_ASM		62
#  define POCC_OPT_CODEGEN_TIMER_PAPI		63
#  define POCC_OPT_COMPILE      		64
#  define POCC_OPT_COMPILE_CMD          	65
#  define POCC_OPT_RUN_CMD_ARGS          	66
#  define POCC_OPT_PROGRAM_TIMEOUT        	67
# endif

BEGIN_C_DECLS

extern
int
pocc_getopts(s_pocc_options_t* options, int argc, char** argv);

END_C_DECLS

#endif // POCC_SRC_OPTIONS_H
