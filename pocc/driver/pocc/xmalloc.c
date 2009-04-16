/*
 * xmalloc.c: this file is part of the PoCC project.
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
# include "config.h"
#endif

#include "common.h"
#include "error.h"

void *
xmalloc (size_t num)
{
  void *new = malloc (num);
  if (! new)
    pocc_fatal ("Memory exhausted");
  return new;
}

void *
xrealloc (void *p, size_t num)
{
  void *new;

  if (! p)
    return xmalloc (num);

  new = realloc (p, num);
  if (! new)
    pocc_fatal ("Memory exhausted");

  return new;
}

void *
xcalloc (size_t num, size_t size)
{
  void *new = xmalloc (num * size);
  bzero (new, num * size);
  return new;
}
