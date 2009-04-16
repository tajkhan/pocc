/*
 * error.h: this file is part of the PoCC project.
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
#ifndef POCC_ERROR_H
# define POCC_ERROR_H 

#include "common.h"

BEGIN_C_DECLS

extern void	set_program_name (const char *argv0);

extern void	pocc_warning (const char *message);
extern void	pocc_error (const char *message);
extern void	pocc_fatal (const char *message);

END_C_DECLS


#endif // POCC_ERROR_H
