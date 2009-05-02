/*
 * driver-pluto.h: this file is part of the PoCC project.
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
#ifndef POCC_DRIVER_PLUTO_H
# define POCC_DRIVER_PLUTO_H

# include <stdio.h>

# if HAVE_CONFIG_H
#  include <pocc-utils/config.h>
# endif

# include <pocc/common.h>
# include <pocc-utils/options.h>
# include <pocc/options.h>

# ifndef SCOPLIB_INT_T_IS_LONGLONG
#  define SCOPLIB_INT_T_IS_LONGLONG
# endif
# include <scoplib/scop.h>

# include <pluto/pluto.h>


BEGIN_C_DECLS

extern
int
pocc_driver_pluto (scoplib_scop_p program,
		   s_pocc_options_t* poptions,
		   s_pocc_utils_options_t* puoptions);


END_C_DECLS


#endif // POCC_DRIVER_PLUTO_H
