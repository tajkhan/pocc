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
#include <libgen.h>
#if HAVE_CONFIG_H
# include <pocc-utils/config.h>
#endif

#include "options.h"


static const struct s_opt       opts[POCC_NB_OPTS] =
{
  { 'h', "help", 0, "\tPrint this help" },
  { 'v', "verbose", 0, "\tVerbose output [off]" },
  { 'o', "output", 1, "\tOutput file [filename.pocc.c]\n" },
  { 'c', "compile", 1, "\tCompilation command [gcc -O3 -lm]\n" },
  { 'l', "letsee", 0, "\tOptimize with LetSee [off]" },
  { '\0', "letsee-searchspace", 1, "LetSee: search space: [precut], multi" },
  { '\0', "letsee-traversal", 1, "LetSee: traversal heuristic:\n\t\t\t\t[exhaust], skip, m1, h1, r1, r1m, h1m" },
  { '\0', "letsee-normspace", 0, "LetSee: normalize search space [off]" },

  { '\0', "letsee-scheme-m1", 0, "LetSee: scheme for M1 traversal [i+p,i,0]" },
  { '\0', "letsee-rtries", 0, "LetSee: number of random draws [50]" },
  { '\0', "letsee-prune-precut", 0, "LetSee: prune precut space" },
  { '\0', "letsee-backtrack", 0, "LetSee: allow bactracking in multi mode\n" },
  { 'p', "pluto", 0, "\tOptimize with PLuTo [off]" },
  { '\0', "pluto-parallel", 0, "PLuTo: OpenMP parallelization [off]" },
  { '\0', "pluto-tile", 0, "\tPLuTo: polyhedral tiling [off]" },
  { '\0', "pluto-l2tile", 0, "PLuTo: perform L2 tiling [off]" },
  { '\0', "pluto-fuse", 1, "\tPLuTo: fusion heuristic:\n\t\t\t\tmaxfuse, [smartfuse], nofuse" },
  { '\0', "pluto-unroll", 0, "PLuTo: unroll loops [off]" },
  { '\0', "pluto-ufactor", 1, "PLuTo: unrolling factor [4]" },
  { '\0', "pluto-polyunroll", 0, "PLuTo: polyhedral unrolling [off]" },
  { '\0', "pluto-prevector", 0, "PLuTo: perform prevectorization [off]" },
  { '\0', "pluto-multipipe", 0, "PLuTo: multipipe [off]" },
  { '\0', "pluto-rar", 0, "\tPLuTo: consider RAR dependences [off]" },
  { '\0', "pluto-lastwriter", 0, "PLuTo: perform lastwriter dep. simp. [off]" },
  { '\0', "pluto-scalpriv", 0, "PLuTo: perform scalar privatization [off]" },
  { '\0', "pluto-bee", 0, "\tPLuTo: use Bee [off]" },
  { '\0', "pluto-quiet", 0, "\tPLuTo: be quiet [off]" },
  { '\0', "pluto-ft", 0, "\tPLuTo: ft [off]" },
  { '\0', "pluto-lt", 0, "\tPLuTo: lt [off]\n" },
  { 'n', "no-codegen", 0, "\tDo not generate code [off]" },
  { '\0', "cloog-cloogf", 0, "CLooG: first level to scan [1]" },
  { '\0', "cloog-cloogl", 0, "CLooG: last level to scan [-1]" },
  { '\0', "codegen-timercode", 0, "Codegen: insert timer code [on]" },
  { '\0', "codegen-timer-asm", 0, "Codegen: insert ASM timer code [off]" },
  { '\0', "codegen-timer-papi", 0, "Codegen: insert PAPI timer code [off]" },

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

  printf("PoCC, the Polyhedral Compiler Collection, version " 
	 PACKAGE_VERSION ".\n\n");
  printf("Written by Louis-Noel Pouchet <" PACKAGE_BUGREPORT ">\n");
  printf("Major contributions by Cedric Bastoul and Uday Bondhugula.\n\n");
  printf("Available options for PoCC are: \n");
  for (i = 0; i < POCC_NB_OPTS; ++i)
    if (opts[i].short_opt != '\0')
      printf ("-%c\t--%s  \t%s\n",
	      opts[i].short_opt,
	      opts[i].long_opt,
	      opts[i].description);
    else
      printf ("  \t--%s  \t%s\n",
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

  // Help.
  if (opt_tab[POCC_OPT_HELP])
    print_help();
  // Input file.
  if (ret)
    {
      options->input_file = fopen (argv[ret], "r");
      if (options->input_file == NULL)
	pocc_usage ();
      options->input_file_name = strdup (argv[ret]);
      if (ret < argc)
	ret = get_cmdline_opts (&option, ret + 1, argc, argv, opt_tab);
    }

  if (ret != argc)
    pocc_usage ();
  // Output file.
  if (opt_tab[POCC_OPT_OUTFILE])
      options->output_file_name = strdup (opt_tab[POCC_OPT_OUTFILE]);
  else
    {
      char buffer[2048];
      strcpy (buffer, options->input_file_name);
      int ii = strlen (buffer) - 1;
      while (buffer[ii--] != '.')
	;
      buffer[++ii] = '\0';
      strcat (buffer, ".pocc.c");
      options->output_file_name = strdup (buffer);
    }
  options->output_file = fopen (options->output_file_name, "w");
  if (options->output_file == NULL)
    pocc_error ("Cannot open output file\n");

  // Verbose.
  if (opt_tab[POCC_OPT_VERBOSE])
    options->verbose = 1;

  // Compile command.
  if (opt_tab[POCC_OPT_COMPILE])
    {
      options->compile_command = strdup (opt_tab[POCC_OPT_COMPILE]);
      options->compile_program = 1;
    }
  else
    options->compile_command = strdup ("gcc -O3 -lm");

  // LetSee options.
  if (opt_tab[POCC_OPT_LETSEE])
    options->letsee = 1;
  if (opt_tab[POCC_OPT_LETSEE_SEARCHSPACE])
    {
      if (! strcmp(opt_tab[POCC_OPT_LETSEE_SEARCHSPACE], "precut"))
	options->letsee_space = LS_TYPE_FS;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_SEARCHSPACE], "multi"))
	options->letsee_space = LS_TYPE_MULTI;
    }
  if (opt_tab[POCC_OPT_LETSEE_TRAVERSAL])
    {
      if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "exhaust"))
	options->letsee_traversal = LS_HEURISTIC_EXHAUST;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "h1"))
	options->letsee_traversal = LS_HEURISTIC_H1;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "r1"))
	options->letsee_traversal = LS_HEURISTIC_R1;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "h1m"))
	options->letsee_traversal = LS_HEURISTIC_H1M;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "r1m"))
	options->letsee_traversal = LS_HEURISTIC_R1M;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "m1"))
	options->letsee_traversal = LS_HEURISTIC_M1;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "skip"))
	options->letsee_traversal = LS_HEURISTIC_SKIP;
    }
  if (opt_tab[POCC_OPT_LETSEE_NORMSPACE])
    options->letsee_normspace = 1;
  if (opt_tab[POCC_OPT_LETSEE_SCHEME_M1])
    {
      char buff[1024];
      int i, j, dim;
      options->letsee_traversal = LS_HEURISTIC_M1;
      options->letsee_scheme_m1 = XMALLOC(int, LS_HEURISTIC_MAX_SCHEME_SIZE);
      for (i = 0; i < LS_HEURISTIC_MAX_SCHEME_SIZE; ++i)
	options->letsee_scheme_m1[i] = LS_HEURISTIC_M1_SCHEME_NONE;
      for (dim = i = j = 0; opt_tab[POCC_OPT_LETSEE_SCHEME_M1][i]; ++dim, j = 0)
	{
	  while (opt_tab[POCC_OPT_LETSEE_SCHEME_M1][i] &&
		 opt_tab[POCC_OPT_LETSEE_SCHEME_M1][i] != ',')
	    buff[j++] = opt_tab[POCC_OPT_LETSEE_SCHEME_M1][i++];
	  buff[j] = '\0';
	  if (! strcmp (buff, "i"))
	    options->letsee_scheme_m1[dim] = LS_HEURISTIC_M1_SCHEME_ITER;
	  else if (! strcmp (buff, "i+p"))
	    options->letsee_scheme_m1[dim] = LS_HEURISTIC_M1_SCHEME_ITERPARAM;
	  else if (! strcmp (buff, "i+p+c"))
	    options->letsee_scheme_m1[dim] = LS_HEURISTIC_M1_SCHEME_FULL;
	  else if (! strcmp (buff, "0"))
	    options->letsee_scheme_m1[dim] = LS_HEURISTIC_M1_SCHEME_NONE;
	  else
	    print_help ();
	  if (opt_tab[POCC_OPT_LETSEE_SCHEME_M1][i])
	    ++i;
	}
    }
  if (opt_tab[POCC_OPT_LETSEE_PRUNE_PRECUT])
    options->letsee_prune_precut = 1;
  if (opt_tab[POCC_OPT_LETSEE_BACKTRACK_MULTI])
    options->letsee_backtrack_multi = 1;
  if (opt_tab[POCC_OPT_LETSEE_RTRIES])
    options->letsee_rtries = atoi (opt_tab[POCC_OPT_LETSEE_RTRIES]);

  // Pluto options.
  if (opt_tab[POCC_OPT_PLUTO])
    options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_PARALLEL])
    options->pluto_parallel = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_TILE])
    options->pluto_tile = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_L2TILE])
    options->pluto_l2tile = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_FUSE])
    {
      if (! strcmp(opt_tab[POCC_OPT_PLUTO_FUSE], "maxfuse"))
	options->pluto_fuse = PLUTO_MAXIMAL_FUSE;
      else if (! strcmp(opt_tab[POCC_OPT_PLUTO_FUSE], "smartfuse"))
	options->pluto_fuse = PLUTO_SMART_FUSE;
      else if (! strcmp(opt_tab[POCC_OPT_PLUTO_FUSE], "nofuse"))
	options->pluto_fuse = PLUTO_NO_FUSE;
      options->pluto = 1;
    }
  if (opt_tab[POCC_OPT_PLUTO_UNROLL])
    options->pluto_unroll = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_UFACTOR]) 
    {
      options->pluto_ufactor = atoi (opt_tab[POCC_OPT_PLUTO_UFACTOR]);
      options->pluto = 1;
    }
  if (opt_tab[POCC_OPT_PLUTO_POLYUNROLL])
    options->pluto_polyunroll = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_PREVECTOR])
    options->pluto_prevector = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_MULTIPIPE])
    options->pluto_multipipe = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_RAR])
    options->pluto_rar = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_LASTWRITER])
    options->pluto_lastwriter = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_SCALPRIV])
    options->pluto_scalpriv = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_BEE])
    options->pluto_bee = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_QUIET])
    options->pluto_quiet = options->pluto = 1;
  if (opt_tab[POCC_OPT_PLUTO_FT])
    {
      options->pluto_ft = atoi (opt_tab[POCC_OPT_PLUTO_FT]);
      options->pluto = 1;
    }
  if (opt_tab[POCC_OPT_PLUTO_LT])
    {
      options->pluto_ft = options->pluto = atoi (opt_tab[POCC_OPT_PLUTO_LT]);
      options->pluto = 1;
    }

  // Codegen/Cloog options.
  if (opt_tab[POCC_OPT_NOCODEGEN])
    options->codegen = 0;
  if (opt_tab[POCC_OPT_CLOOG_F])
    options->cloog_f = atoi (opt_tab[POCC_OPT_CLOOG_F]);
  if (opt_tab[POCC_OPT_CLOOG_L])
    options->cloog_f = atoi (opt_tab[POCC_OPT_CLOOG_L]);
  if (opt_tab[POCC_OPT_CODEGEN_TIMERCODE])
    options->codegen_timercode = 1;
  if (opt_tab[POCC_OPT_CODEGEN_TIMER_ASM])
    options->codegen_timer_asm = 1;
  if (opt_tab[POCC_OPT_CODEGEN_TIMER_PAPI])
    options->codegen_timer_papi = 1;

  free (opt_tab);
  return 0;
}
