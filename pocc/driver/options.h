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
#ifndef POCC_BIN_OPTIONS_H
# define POCC_BIN_OPTIONS_H

# if HAVE_CONFIG_H
#  include "config.h"
# endif

# include "common.h"
# include "getopts.h"
//# include <pocc/options.h>


# define LS_NB_OPTS                26
# define LS_OPT_LB                  0
# define LS_OPT_UB                  1
# define LS_OPT_PLB                 2
# define LS_OPT_PUB                 3
# define LS_OPT_CLB                 4
# define LS_OPT_CUB                 5
# define LS_OPT_OUTPUT              6
# define LS_OPT_CREATE_SCHEDFILES   7
# define LS_OPT_TRANSFO_DIR         8
//# define LS_OPT_FM_MODE             9
# define LS_OPT_TYPE               9
# define LS_OPT_HEURISTIC          10
# define LS_OPT_SPACENORM          11
# define LS_OPT_COMPILE_LINE       12
# define LS_OPT_H1_THRESOLD        13
# define LS_OPT_R_TRIES		   14
# define LS_OPT_LEFUR              15
# define LS_OPT_MAXSCALE           16
# define LS_OPT_LOADSPACE          17
# define LS_OPT_VECTOR             18
# define LS_OPT_VECTOR2            19
# define LS_OPT_BACKTRACK          20
# define LS_OPT_TRAFFIC            21
# define LS_OPT_SCHEME_M1          22
# define LS_OPT_PRUNE_OSET         23
# define LS_OPT_VERBOSE            24
# define LS_OPT_HELP               25


int
pocc_getopts(s_ls_options_t* options, int argc, char** argv);


#endif // POCC_BIN_OPTIONS_H
