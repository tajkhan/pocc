/*
 * driver-letsee.h: this file is part of the PoCC project.
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
#ifndef POCC_DRIVER_LETSEE_H
# define POCC_DRIVER_LETSEE_H

# include <stdio.h>

# if HAVE_CONFIG_H
#  include <pocc-utils/config.h>
# endif
# define LINEAR_VALUE_IS_LONGLONG
# include <pocc/common.h>
# include <pocc/driver-codegen.h>
# include <pocc-utils/options.h>
# include <pocc/options.h>


BEGIN_C_DECLS

extern
void
pocc_driver_letsee (clan_scop_p program,
		    s_pocc_options_t* poptions,
		    s_pocc_utils_options_t* puoptions);

END_C_DECLS


#endif // POCC_DRIVER_LETSEE_H
