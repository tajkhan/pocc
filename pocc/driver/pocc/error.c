/*
 * error.c: this file is part of the PoCC project.
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
#if HAVE_CONFIG_H
# include <pocc-utils/config.h>
#endif

#include <pocc/common.h>
#include <pocc/error.h>

char* basename (const char*);

static void error (int exit_status, const char *mode, 
		   const char *message);

static const char *program_name = NULL;


void
pocc_set_program_name (const char *path)
{
  if (! program_name)
    program_name = xstrdup (basename (path));
}

static void
error (int exit_status, const char *mode, const char *message)
{
  fprintf (stderr, "%s: %s: %s.\n", program_name, mode, message);

  if (exit_status >= 0)
    exit (exit_status);
}

void
pocc_warning (const char *message)
{
  error (-1, "warning", message);
}

void
pocc_error (const char *message)
{
  error (-1, "ERROR", message);
}

void
pocc_fatal (const char *message)
{
  error (EXIT_FAILURE, "FATAL", message);
}
