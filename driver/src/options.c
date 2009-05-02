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
  { 'h', "help", 0, "\t\tPrint this help" },
  { 'v', "version", 0, "\tPrint version information" },
  { 'o', "output", 1, "\tOutput file [filename.pocc.c]\n" },
  { '\0', "verbose", 0, "\tVerbose output [off]" },
  { '\0', "quiet", 0, "\tMinimal output [off]\n" },
  { 'l', "letsee", 0, "\tOptimize with LetSee [off]" },
  { '\0', "letsee-space", 1, "LetSee: search space: [precut], schedule" },
  { '\0', "letsee-walk", 1, "LetSee: traversal heuristic:\n\t\t\t\t[exhaust], random, skip, m1, dh, ga" },
  { '\0', "letsee-dry-run", 0, "Only generate source files [off]" },
  { '\0', "letsee-normspace", 0, "LetSee: normalize search space [off]" },

  { '\0', "letsee-mode-m1", 1, "LetSee: scheme for M1 traversal [i+p,i,0]" },
  { '\0', "letsee-rtries", 1, "LetSee: number of random draws [50]" },
  { '\0', "letsee-prune-precut", 0, "LetSee: prune precut space" },
  { '\0', "letsee-backtrack", 0, "LetSee: allow bactracking in schedule mode\n" },
  { 'p', "pluto", 0, "\tOptimize with PLuTo [off]" },
  { '\0', "pluto-parallel", 0, "PLuTo: OpenMP parallelization [off]" },
  { '\0', "pluto-tile", 0, "\tPLuTo: polyhedral tiling [off]" },
  { '\0', "pluto-l2tile", 0, "\tPLuTo: perform L2 tiling [off]" },
  { '\0', "pluto-fuse", 1, "PLuTo: fusion heuristic:\n\t\t\t\tmaxfuse, [smartfuse], nofuse" },
  { '\0', "pluto-unroll", 0, "\tPLuTo: unroll loops [off]" },
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
  { '\0', "cloog-cloogf", 1, "CLooG: first level to scan [1]" },
  { '\0', "cloog-cloogl", 1, "CLooG: last level to scan [-1]" },
  { '\0', "codegen-timercode", 0, "Codegen: insert timer code [off]" },
  { '\0', "codegen-timer-asm", 0, "Codegen: insert ASM timer code [off]" },
  { '\0', "codegen-timer-papi", 0, "Codegen: insert PAPI timer code [off]\n" },
  { 'c', "compile", 0, "\tCompile program with C compiler [off]" },
  { '\0', "compile-cmd", 1, "Compilation command [gcc -O3 -lm]" }

};


static const struct s_option    option =
  {
    opts,
    {"Unknown argument: -", "unknown argument: --",
     "Expected argument for option: "},
    POCC_NB_OPTS,
    1
  };


static void     print_version (void)
{
  printf ("PoCC, the Polyhedral Compiler Collection, version "
	 PACKAGE_VERSION ".\n\n");
  printf ("Written by Louis-Noel Pouchet <" PACKAGE_BUGREPORT ">\n");
  printf ("Major contributions by Cedric Bastoul and Uday Bondhugula.\n\n");
  printf("PoCC packages several free software:\n");
  printf ("* Clan \t\thttp://www.lri.fr/~bastoul/development/clan\n");
  printf ("* Candl \thttp://www.lri.fr/~bastoul/development/candl\n");
  printf ("* LetSee \thttp://www-rocq.inria.fr/~pouchet/software/letsee\n");
  printf ("* PLuTo \thttp://www.cse.ohio-state.edu/~bondhugu/pluto\n");
  printf ("* CLooG \thttp://www.cloog.org\n");
  printf ("* PIPLib \thttp://www.piplib.org\n");
  printf ("* PolyLib \thttp://icps.u-strasbg.fr/polylib\n");
  printf ("* FM \t\thttp://www-rocq.inria.fr/~pouchet/software/fm\n");
  exit (1);
}


static void     print_help (void)
{
  int           i;

  printf ("PoCC, the Polyhedral Compiler Collection, version "
	 PACKAGE_VERSION ".\n\n");
  printf ("Written by Louis-Noel Pouchet <" PACKAGE_BUGREPORT ">\n");
  printf ("Major contributions by Cedric Bastoul and Uday Bondhugula.\n\n");

  printf("Available options for PoCC are: \n");
  for (i = 0; i < POCC_NB_OPTS; ++i)
    if (opts[i].short_opt != '\0')
      {
	if (opts[i].expect_arg)
	  printf ("-%c\t--%s <arg> \t%s\n",
		  opts[i].short_opt,
		  opts[i].long_opt,
		  opts[i].description);
	else
	  printf ("-%c\t--%s \t%s\n",
		  opts[i].short_opt,
		  opts[i].long_opt,
		  opts[i].description);
      }
    else
      {
	if (opts[i].expect_arg)
	  printf ("  \t--%s <arg> \t%s\n",
		  opts[i].long_opt,
		  opts[i].description);
	else
	  printf ("  \t--%s \t%s\n",
		  opts[i].long_opt,
		  opts[i].description);
      }
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
    print_help ();
  // Version.
  if (opt_tab[POCC_OPT_VERSION])
    print_version ();

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

  // Help, again.
  if (opt_tab[POCC_OPT_HELP])
    print_help ();
  // Version.
  if (opt_tab[POCC_OPT_VERSION])
    print_version ();
  // Verbose.
  if (opt_tab[POCC_OPT_VERBOSE])
    options->verbose = 1;
  // Quiet.
  if (opt_tab[POCC_OPT_QUIET])
    {
      options->quiet = 1;
      options->pluto_quiet = 1;
      options->verbose = 0;
    }

  // LetSee options.
  if (opt_tab[POCC_OPT_LETSEE])
    options->letsee = 1;
  if (opt_tab[POCC_OPT_LETSEE_SEARCHSPACE])
    {
      if (! strcmp(opt_tab[POCC_OPT_LETSEE_SEARCHSPACE], "precut"))
	options->letsee_space = LS_TYPE_FS;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_SEARCHSPACE], "schedule"))
	options->letsee_space = LS_TYPE_MULTI;
      options->letsee = 1;
    }
  if (opt_tab[POCC_OPT_LETSEE_TRAVERSAL])
    {
      if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "exhaust"))
	options->letsee_traversal = LS_HEURISTIC_EXHAUST;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "dh"))
	options->letsee_traversal = LS_HEURISTIC_DH;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "random"))
	options->letsee_traversal = LS_HEURISTIC_RANDOM;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "m1"))
	options->letsee_traversal = LS_HEURISTIC_M1;
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "skip"))
	options->letsee_traversal = LS_HEURISTIC_SKIP;
      options->letsee = 1;
    }
  if (opt_tab[POCC_OPT_LETSEE_NORMSPACE])
    {
      options->letsee_normspace = 1;
      options->letsee = 1;
    }
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
      options->letsee = 1;
    }
  if (opt_tab[POCC_OPT_LETSEE_PRUNE_PRECUT])
    options->letsee_prune_precut = options->letsee = 1;
  if (opt_tab[POCC_OPT_LETSEE_BACKTRACK_MULTI])
    options->letsee_backtrack_multi = options->letsee = 1;
  if (opt_tab[POCC_OPT_LETSEE_RTRIES])
    {
      options->letsee_rtries = atoi (opt_tab[POCC_OPT_LETSEE_RTRIES]);
      options->letsee = 1;
    }
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

  // Compile.
  if (options->letsee || opt_tab[POCC_OPT_COMPILE])
    options->compile_program = 1;
  // Compile command.
  if (opt_tab[POCC_OPT_COMPILE_CMD])
    {
      options->compile_command = strdup (opt_tab[POCC_OPT_COMPILE]);
      options->compile_program = 1;
    }
  else
    options->compile_command = strdup ("gcc -O3 -lm");

  if (opt_tab[POCC_OPT_LETSEE_DRY_RUN])
    {
      options->compile_program = 0;
      options->letsee = 1;
    }

  // Codegen/Cloog options.
  if (opt_tab[POCC_OPT_NOCODEGEN])
    {
      options->codegen = 0;
      options->compile_program = 0;
    }
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

  // Letsee precut mode implies Pluto mode.
  if (options->letsee && options->letsee_space == LS_TYPE_FS)
    options->pluto = 1;

  // Deal with opening the output file.
  if (! options->letsee && options->codegen)
    {
      options->output_file = fopen (options->output_file_name, "w");
      if (options->output_file == NULL)
	pocc_error ("Cannot open output file\n");
    }      
  
  free (opt_tab);
  return 0;
}
