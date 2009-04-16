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
# include "config.h"
#endif

#include "options.h"


static const struct s_opt       opts[LS_NB_OPTS] =
{
  { 'a', "ilb", 1, "\tLower iterator coefficient bound [-1]" },
  { 'b', "iub", 1, "\tUpper iterator coefficient bound [ 1]" },
  { 'c', "plb", 1, "\tLower parameter coefficient bound [-1]" },
  { 'd', "pub", 1, "\tUpper parameter coefficient bound [ 1]" },
  { 'e', "clb", 1, "\tLower constant coefficient bound [-1]" },
  { 'f', "cub", 1, "\tUpper constant coefficient bound [ 1]" },
  { 'o', "output", 1, "Output file [stdout]" },
  { 'n', "no-files", 0, "Don't create CLooG files for drawn schedules" },
  { 'z', "transfo-dir", 1, "Directory for CLooG files [transformations]" },
  { 't', "type", 1, "Space computation algorithm ([ospace], multi, fusion, fd, fs)"},
  { 'w', "walk", 1, "Exploration heuristic ([exhaust], h1, r1, r1m, m1, \n\t\t\tpluto, plutocc)"},
  { 's', "spacenorm", 0, "Normalize legal space" },
  { 'x', "execute", 1, "Compilation line" },
  { 'r', "rate", 1, "Thresold for heuristic filter (in %) [5]" },
  { 'q', "rtries", 1, "Number of tries for heuristic [20]" },
  { 'l', "le-fur", 0, "Use Le Fur redundancy elimination" },
  { 'M', "maxscale", 0, "Use maximum scalability mode to solve systems" },
  { 'L', "load-space", 1, "Load space from file" },
  { 'j', "vector", 1, "Prefix vector for schedule" },
  { 'i', "vector2", 1, "Prefix vector2 for schedule" },
  { 'y', "backtrack", 0, "Enable backtrack mode" },
  { 'p', "dep-order", 0, "Enable dependence ordering w.r.t. traffic" },
  { 'S', "scheme-m1", 1, "Specify scheme (imply -w m1): \"i+p+c,i+p,i,0\"" },
  { 'P', "prune-oset", 0, "Prune OSet, for fs space mode" },
  { 'v', "verbose", 0, "Verbose output" },
  { 'h', "help", 0, "Print this help" }
};


static const struct s_option    option =
  {
    opts,
    {"Unknown argument: -", "unknown argument: --",
     "Expected argument for option: "},
    LS_NB_OPTS,
    1
  };


static void     print_help (void)
{
  int           i;

  printf("Options for pocc are: \n");
  for (i = 0; i < LS_NB_OPTS; ++i)
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
pocc_getopts (void* options, int argc, char** argv)
{
  char          **opt_tab;
  int		ret;
  unsigned	i;

  opt_tab = malloc (sizeof (char *) * LS_NB_OPTS);
  for (i = 0; i < LS_NB_OPTS; ++i)
    opt_tab[i] = NULL;
  ret = get_cmdline_opts (&option, 1, argc, argv, opt_tab);

/*   if (opt_tab[LS_OPT_HELP]) */
/*     print_help(); */
/*   if (ret) */
/*     { */
/*       options->in_file = fopen (argv[ret], "r"); */
/*       if (options->in_file == NULL) */
/* 	ls_usage (); */
/*       options->input_file = strdup (argv[ret]); */
/*       if (ret < argc) */
/* 	ret = get_cmdline_opts (&option, ret + 1, argc, argv, opt_tab); */
/*     } */

/*   if (opt_tab[LS_OPT_HELP]) */
/*     print_help(); */
/*   if (ret != argc) */
/*     ls_usage (); */

/*   if (opt_tab[LS_OPT_LB]) */
/*     Z_ASSIGN_SI(options->lb, atoi (opt_tab[LS_OPT_LB])); */

  free (opt_tab);
  return 0;
}
