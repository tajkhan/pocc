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
#ifndef POCC_OPTIONS_H
# define POCC_OPTIONS_H

# include <stdio.h>
# include <pocc/common.h>
# include <cloog/options.h>


struct s_pocc_options
{
  FILE*		input_file;
  char*		input_file_name;
  FILE*		output_file;
  char*		output_file_name;
  int		verbose;
  CloogOptions*	cloog_options;

  int		vectorize;
  int		parallel;
  int		unroll;
  int		timer;

};
typedef struct s_pocc_options s_pocc_options_t;



BEGIN_C_DECLS

extern
s_pocc_options_t* pocc_options_malloc ();

extern
void pocc_options_init_cloog (s_pocc_options_t* options);

extern
void pocc_options_free (s_pocc_options_t* options);


END_C_DECLS



#endif // POCC_OPTIONS_H
