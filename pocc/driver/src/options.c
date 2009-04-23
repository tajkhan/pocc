/*
 * options.c: this file is part of the PoCC project.
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

#include "options.h"


static const struct s_opt       opts[POCC_NB_OPTS] =
{
  { 'v', "verbose", 0, "Verbose output" },
  { 'h', "help", 0, "Print this help" },
  { 'o', "output", 1, "Output file" }
};


static const struct s_option    option =
  {
    opts,
    {"Unknown argument: -", "unknown argument: --",
     "Expected argument for option: "},
    POCC_NB_OPTS,
    1
  };


static void     print_help (void)
{
  int           i;

  printf("Options for pocc are: \n");
  for (i = 0; i < POCC_NB_OPTS; ++i)
    printf("-%c\t--%s  \t%s\n",
           opts[i].short_opt,
           opts[i].long_opt,
           opts[i].description);
  exit (1);
}


void
pocc_usage ()
{
  fprintf (stderr, "Usage: pocc [options] source-file\n");
  exit (1);
}


int
pocc_getopts (s_pocc_options_t* options, int argc, char** argv)
{
  char          **opt_tab;
  int		ret;
  unsigned	i;

  opt_tab = malloc (sizeof (char *) * POCC_NB_OPTS);
  for (i = 0; i < POCC_NB_OPTS; ++i)
    opt_tab[i] = NULL;
  ret = get_cmdline_opts (&option, 1, argc, argv, opt_tab);

  if (opt_tab[POCC_OPT_HELP])
    print_help();
  if (ret)
    {
      options->input_file = fopen (argv[ret], "r");
      if (options->input_file == NULL)
	pocc_usage ();
      options->input_file_name = strdup (argv[ret]);
      if (ret < argc)
	ret = get_cmdline_opts (&option, ret + 1, argc, argv, opt_tab);
    }

  if (opt_tab[POCC_OPT_HELP])
    print_help();
  if (ret != argc)
    pocc_usage ();

  if (opt_tab[POCC_OPT_VERBOSE])
    options->verbose = 1;

  free (opt_tab);
  return 0;
}
