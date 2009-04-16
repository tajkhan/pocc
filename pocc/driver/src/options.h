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

# include "getopts.h"


# define LS_NB_OPTS                26

struct s_pocc_option
{
  

};
typedef struct s_pocc_option s_pocc_option_t;


int
pocc_getopts(void* options, int argc, char** argv);


#endif // POCC_BIN_OPTIONS_H
