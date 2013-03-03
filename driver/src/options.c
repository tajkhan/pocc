/*
 * options.c: this file is part of the PoCC project.
 *
 * PoCC, the Polyhedral Compiler Collection package
 *
 * Copyright (C) 2009 Louis-Noel Pouchet
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * The complete GNU General Public Licence Notice can be found as the
 * `COPYING.LESSER' file in the root directory.
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

#ifndef POCC_RELEASE_MODE
static const struct s_opt       opts[POCC_NB_OPTS] =
{
  { 'h', "help", 0, "Print this help" , "\t\t(S)" },
  { 'v', "version", 0, "Print version information" , "\t(S)" },
  { 'o', "output", 1, "Output file [filename.pocc.c]" , "\t(S)" },
  { '\0', "output-scop", 0, "Output scoplib file to\n\t\t\t\t\tfilename.pocc.scop" , "\t(S)" },
  { '\0', "cloogify-scheds", 0, "Create CLooG-compatible schedules in\n\t\t\t\t\tthe scop" , "(S)" },
  { '\0', "bounded-ctxt", 0, "Parser: bound all global parameters\n\t\t\t\t\tto >= -1" , "\t(S)" },
  { '\0', "default-ctxt", 0, "Parser: bound all global parameters\n\t\t\t\t\tto>= 32" , "\t(S)" },
  { '\0', "inscop-fakearray", 0, "Parser: use FAKEARRAY[i] to explicitly\n\t\t\t\t\tdeclare write dependences" , "(E)" },
  { '\0', "read-scop", 0, "Parser: read SCoP file instead of C file\n\t\t\t\t\tas input\n" , "\t(S)" },
  { '\0', "no-candl", 0, "Dependence analysis: don't run candl" , "\t(S)" },
  { '\0', "candl-dep-isl-simp", 0, "Dependence analysis: simplify with ISL" , "(E)" },
  { '\0', "candl-dep-prune", 0, "Dependence analysis: prune redundant\n\t\t\t\t\tdeps\n" , "(E)" },
  { '\0', "polyfeat", 0, "Run Polyhedral Feature Extraction" , "\t(E)" },
  { '\0', "polyfeat-rar", 0, "Consider RAR dependences in PolyFeat\n" , "\t(E)" },
  { 'd', "delete-files", 0, "Delete files previously generated\n\t\t\t\t\tby PoCC\n" , "\t(S)" },
  { '\0', "verbose", 0, "Verbose output" , "\t(S)" },
  { '\0', "quiet", 0, "Minimal output\n" , "\t(S)" },
  { 'l', "letsee", 0, "Optimize with LetSee" , "\t(S)" },
  { '\0', "letsee-space", 1, "LetSee: search space: [precut], schedule" , "(E)" },
  { '\0', "letsee-walk", 1, "LetSee: traversal heuristic:\n\t\t\t\t\t[exhaust], random, skip, m1, dh, ga" , "(B)" },
  { '\0', "letsee-dry-run", 0, "Only generate source files" , "(S)" },
  { '\0', "letsee-normspace", 0, "LetSee: normalize search space" , "(E)" },
  { '\0', "letsee-bounds", 1, "LetSee: search space bounds\n\t\t\t\t\t[-1,1,-1,1,-1,1]" , "(E)" },
  { '\0', "letsee-mode-m1", 1, "LetSee: scheme for M1 traversal\n\t\t\t\t\t[i+p,i,0]" , "(E)" },
  { '\0', "letsee-rtries", 1, "LetSee: set number of random draws [50]" , "(E)" },
  { '\0', "letsee-prune-precut", 0, "LetSee: prune precut space" , "(E)" },
  { '\0', "letsee-backtrack", 0, "LetSee: allow bactracking in\n\t\t\t\t\tschedule mode\n" , "(E)" },
  { 'p', "pluto", 0, "Optimize with PLuTo" , "\t(S)" },
  { '\0', "pluto-parallel", 0, "PLuTo: Wavefront parallelization " , "(S)" },
  { '\0', "pluto-tile", 0, "PLuTo: polyhedral tiling" , "\t(S)" },
  { '\0', "pluto-l2tile", 0, "PLuTo: perform L2 tiling" , "\t(E)" },
  { '\0', "pluto-fuse", 1, "PLuTo: fusion heuristic:\n\t\t\t\t\tmaxfuse, [smartfuse], nofuse" , "(S)" },
  { '\0', "pluto-unroll", 0, "PLuTo: unroll loops" , "\t(E)" },
  { '\0', "pluto-ufactor", 1, "PLuTo: unrolling factor [4]" , "(E)" },
  { '\0', "pluto-polyunroll", 0, "PLuTo: polyhedral unrolling" , "(B)" },
  { '\0', "pluto-prevector", 0, "PLuTo: perform prevectorization" , "(S)" },
  { '\0', "pluto-multipipe", 0, "PLuTo: multipipe" , "(E)" },
  { '\0', "pluto-rar", 0, "PLuTo: consider RAR dependences" , "\t(S)" },
  { '\0', "pluto-rar-cf", 0, "PLuTo: consider RAR dependences for \n\t\t\t\t\tcost function only" , "\t(S)" },
  { '\0', "pluto-lastwriter", 0, "PLuTo: perform lastwriter dependence\n\t\t\t\t\tsimplification" , "(E)" },
  { '\0', "pluto-scalpriv", 0, "PLuTo: perform scalar privatization" , "(S)" },
  { '\0', "pluto-bee", 0, "PLuTo: use Bee" , "\t(B)" },
  { '\0', "pluto-quiet", 0, "PLuTo: be quiet" , "\t(S)" },
  { '\0', "pluto-ft", 0, "PLuTo: CLooG ft " , "\t(S)" },
  { '\0', "pluto-lt", 0, "PLuTo: CLooG lt" , "\t(S)" },
  { '\0', "pluto-ext-candl", 0, "PLuTo: Read dependences from SCoP" , "(S)" },
  { '\0', "pluto-tile-scat", 0, "PLuTo: Perform tiling inside scatterings" , "(S)" },
  { '\0', "pluto-bounds", 1, "PLuTo: Transformation coefficients\n\t\t\t\t\tbounds [+inf]\n" , "(S)" },

  { 'n', "no-codegen", 0, "Do not generate code" , "\t(S)" },
  { '\0', "cloog-cloogf", 1, "CLooG: first level to scan [1]" , "(S)" },
  { '\0', "cloog-cloogl", 1, "CLooG: last level to scan [-1]" , "(S)" },
  { '\0', "print-cloog-file", 0, "CLooG: print input CLooG file" , "(S)" },
  { '\0', "no-past", 0, "Do not use the PAST back-end" , "\t(E)" },
  { '\0', "past-hoist-lb", 0, "Hoist loop bounds" , "(S)" },
  { '\0', "pragmatizer", 0, "Use the AST pragmatizer" , "\t(S)" },
  { '\0', "ptile", 0, "Use PTile for parametric tiling" , "\t(S)" },
  { '\0', "ptile-fts", 0, "Use full-tile separation in PTile" , "\t(S)" },
  { '\0', "punroll", 0, "Use PAST loop unrolling" , "\t(S)" },
  { '\0', "register-tiling", 0, "PAST register tiling" , "(E)" },
  { '\0', "punroll-size", 1, "PAST unrolling size [4]" , "(S)" },
  { '\0', "vectorizer", 0, "Post-transform for vectorization" , "\t(S)" },
  { '\0', "mark-par-loops", 0, "Detect parallel loops in generated\n\t\t\t\t\tcode" , "(E)" },
  { '\0', "no-outer-par-loops", 0, "Don't preserve outer parallel loop\n\t\t\t\t\tduring vectorization" , "(E)" },
  { '\0', "sink-all-loops", 0, "Sink all candidate vector loops during\n\t\t\t\t\tvectorization" , "(E)" },
  { '\0', "array-contraction", 0, "Perform array contraction [off]" , "(E)" },
  { '\0', "ac-keep-outer-par", 0, "Array contraction: preserve outer\n\t\t\t\t\tparallel loops" , "(E)" },
  { '\0', "ac-keep-vector-par", 0, "Array contraction: preserve inner\n\t\t\t\t\tparallel loops" , "(E)" },
  { '\0', "codegen-timercode", 0, "Codegen: insert timer code" , "(E)" },
  { '\0', "codegen-timer-asm", 0, "Codegen: insert ASM timer code" , "(E)" },
  { '\0', "codegen-timer-papi", 0, "Codegen: insert PAPI timer code\n" , "(E)" },
  { 'c', "compile", 0, "Compile program with C compiler" , "\t(S)" },
  { '\0', "compile-cmd", 1, "Compilation command [gcc -O3 -lm]" , "(S)" },
  { '\0', "run-cmd-args", 1, "Program execution arguments []" , "(S)" },
  { '\0', "prog-timeout", 1, "Timeout for compilation and execution,\n\t\t\t\t\tin second\n", "(E)" },

  { 's', "ponos", 0, "Optimize with Ponos" , "\t(E)" },
  { '\0', "ponos-quiet", 0, "Ponos: be quiet" , "\t(E)" },
  { '\0', "ponos-debug", 0, "Ponos: debug mode" , "\t(E)" },
  { '\0', "ponos-sched-sz", 1, "Ponos: schedule dimensionality [1]" , "(E)" },
  { '\0', "ponos-coef-pos", 0, "Ponos: schedule coefficients are >= 0" , "(E)" },
  { '\0', "ponos-build-2dp1", 0, "Ponos: build 2d+1 schedule" , "(E)" },
  { '\0', "ponos-solver", 1, "Ponos: solver: [pip]" , "(E)" },
  { '\0', "ponos-farkas-max", 0, "Ponos: maxscale FM solver" , "(E)" },
  { '\0', "ponos-farkas-nored", 0, "Ponos: remove redundancy with FM" , "(E)" },
  { '\0', "ponos-K", 1, "Ponos: value for the K constant [10]" , "(E)" },
  { '\0', "ponos-coef", 1, "Ponos: schedule coefficients bound [10]\n" , "(E)" },
  { '\0', "past-super-hoist", 0, "Hoist loop bounds (super aggresive)" , "(E)" }

};

#else
static const struct s_opt       opts[POCC_NB_OPTS] =
{
  { 'h', "help", 0, "Print this help" , "\t\t(S)" },
  { 'v', "version", 0, "Print version information" , "\t(S)" },
  { 'o', "output", 1, "Output file [filename.pocc.c]" , "\t(S)" },
  { '\0', "output-scop", 0, "Output scoplib file to\n\t\t\t\t\tfilename.pocc.scop" , "\t(S)" },
  { '\0', "cloogify-scheds", 0, "Create CLooG-compatible schedules in\n\t\t\t\t\tthe scop" , "(S)" },
  { '\0', "bounded-ctxt", 0, "Parser: bound all global parameters\n\t\t\t\t\tto >= -1" , "\t(S)" },
  { '\0', "default-ctxt", 0, "Parser: bound all global parameters\n\t\t\t\t\tto>= 32" , "\t(S)" },
  { '\0', "inscop-fakearray", 0, "Parser: use FAKEARRAY[i] to explicitly\n\t\t\t\t\tdeclare write dependences" , "(E)" },
  { '\0', "read-scop", 0, "Parser: read SCoP file instead of C file\n\t\t\t\t\tas input\n" , "\t(S)" },
  { '\0', "no-candl", 0, "Dependence analysis: don't run candl" , "\t(S)" },
  { '\0', "candl-dep-isl-simp", 0, "Dependence analysis: simplify with ISL" , "(E)" },
  { '\0', "candl-dep-prune", 0, "Dependence analysis: prune redundant\n\t\t\t\t\tdeps\n" , "(E)" },
  { '\0', "polyfeat", 0, "Run Polyhedral Feature Extraction" , "\t(E)" },
  { '\0', "polyfeat-rar", 0, "Consider RAR dependences in PolyFeat\n" , "\t(E)" },
  { 'd', "delete-files", 0, "Delete files previously generated\n\t\t\t\t\tby PoCC\n" , "\t(S)" },
  { '\0', "verbose", 0, "Verbose output" , "\t(S)" },
  { '\0', "quiet", 0, "Minimal output\n" , "\t(S)" },

  { 'l', "letsee", 0, "Optimize with LetSee" , "\t(S)" },
  { '\0', "letsee-space", 1, "LetSee: search space: [precut], schedule" , "(E)" },
  { '\0', "letsee-walk", 1, "LetSee: traversal heuristic:\n\t\t\t\t\t[exhaust], random, skip, m1, dh, ga" , "(B)" },
  { '\0', "letsee-dry-run", 0, "Only generate source files" , "(S)" },
  { '\0', "letsee-normspace", 0, "LetSee: normalize search space" , "(E)" },
  { '\0', "letsee-bounds", 1, "LetSee: search space bounds\n\t\t\t\t\t[-1,1,-1,1,-1,1]" , "(E)" },
  { '\0', "letsee-mode-m1", 1, "LetSee: scheme for M1 traversal\n\t\t\t\t\t[i+p,i,0]" , "(E)" },
  { '\0', "letsee-rtries", 1, "LetSee: set number of random draws [50]" , "(E)" },
  { '\0', "letsee-prune-precut", 0, "LetSee: prune precut space" , "(E)" },
  { '\0', "letsee-backtrack", 0, "LetSee: allow bactracking in\n\t\t\t\t\tschedule mode\n" , "(E)" },

  { 'p', "pluto", 0, "Optimize with PLuTo" , "\t(S)" },
  { '\0', "pluto-parallel", 0, "PLuTo: Wavefront parallelization " , "(S)" },
  { '\0', "pluto-tile", 0, "PLuTo: polyhedral tiling" , "\t(S)" },
  { '\0', "pluto-l2tile", 0, "PLuTo: perform L2 tiling" , "\t(E)" },
  { '\0', "pluto-fuse", 1, "PLuTo: fusion heuristic:\n\t\t\t\t\tmaxfuse, [smartfuse], nofuse" , "(S)" },
  { '\0', "pluto-unroll", 0, "PLuTo: unroll loops" , "\t(E)" },
  { '\0', "pluto-ufactor", 1, "PLuTo: unrolling factor [4]" , "(E)" },
  { '\0', "pluto-polyunroll", 0, "PLuTo: polyhedral unrolling" , "(B)" },
  { '\0', "pluto-prevector", 0, "PLuTo: perform prevectorization" , "(S)" },
  { '\0', "pluto-multipipe", 0, "PLuTo: multipipe" , "(E)" },
  { '\0', "pluto-rar", 0, "PLuTo: consider RAR dependences" , "\t(S)" },
  { '\0', "pluto-rar-cf", 0, "PLuTo: consider RAR dependences for \n\t\t\t\t\tcost function only" , "\t(S)" },
  { '\0', "pluto-lastwriter", 0, "PLuTo: perform lastwriter dependence\n\t\t\t\t\tsimplification" , "(E)" },
  { '\0', "pluto-scalpriv", 0, "PLuTo: perform scalar privatization" , "(S)" },
  { '\0', "pluto-bee", 0, "PLuTo: use Bee" , "\t(B)" },
  { '\0', "pluto-quiet", 0, "PLuTo: be quiet" , "\t(S)" },
  { '\0', "pluto-ft", 0, "PLuTo: CLooG ft " , "\t(S)" },
  { '\0', "pluto-lt", 0, "PLuTo: CLooG lt" , "\t(S)" },
  { '\0', "pluto-ext-candl", 0, "PLuTo: Read dependences from SCoP" , "(S)" },
  { '\0', "pluto-tile-scat", 0, "PLuTo: Perform tiling inside scatterings" , "(S)" },
  { '\0', "pluto-bounds", 1, "PLuTo: Transformation coefficients\n\t\t\t\t\tbounds [+inf]\n" , "(S)" },


  { 'n', "no-codegen", 0, "Do not generate code" , "\t(S)" },
  { '\0', "cloog-cloogf", 1, "CLooG: first level to scan [1]" , "(S)" },
  { '\0', "cloog-cloogl", 1, "CLooG: last level to scan [-1]" , "(S)" },
  { '\0', "print-cloog-file", 0, "CLooG: print input CLooG file" , "(S)" },
  { '\0', "no-past", 0, "Do not use the PAST back-end" , "\t(E)" },
  { '\0', "past-hoist-lb", 0, "Hoist loop bounds" , "(S)" },
  { '\0', "pragmatizer", 0, "Use the AST pragmatizer" , "\t(S)" },
  { '\0', "ptile", 0, "Use PTile for parametric tiling" , "\t(S)" },
  { '\0', "ptile-fts", 0, "Use full-tile separation in PTile" , "\t(S)" },
  { '\0', "punroll", 0, "Use PAST loop unrolling" , "\t(S)" },
  { '\0', "register-tiling", 0, "PAST register tiling" , "(E)" },
  { '\0', "punroll-size", 1, "PAST unrolling size [4]" , "(S)" },
  { '\0', "vectorizer", 0, "Post-transform for vectorization" , "\t(S)" },

  { '\0', "codegen-timercode", 0, "Codegen: insert timer code" , "(E)" },
  { '\0', "codegen-timer-asm", 0, "Codegen: insert ASM timer code" , "(E)" },
  { '\0', "codegen-timer-papi", 0, "Codegen: insert PAPI timer code\n" , "(E)" },
  { 'c', "compile", 0, "Compile program with C compiler" , "\t(S)" },
  { '\0', "compile-cmd", 1, "Compilation command [gcc -O3 -lm]" , "(S)" },
  { '\0', "run-cmd-args", 1, "Program execution arguments []" , "(S)" },
  { '\0', "prog-timeout", 1, "Timeout for compilation and execution,\n\t\t\t\t\tin second", "(E)" }

};


#endif


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
  printf("PoCC packages several Free Software:\n");
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
  printf ("Major contributions by Cedric Bastoul, Uday Bondhugula, ");
  printf ("and Sven Verdoolaege.\n\n");

  printf("Available options for PoCC are: \n");
  printf("(S) -> stable option, should work as expected\n");
  printf("(E) -> experimental option, use at your own risk\n");
  printf("(B) -> broken option, will likely not work\n\n");
  printf("\n");
  printf("short\t\tlong\t\tstatus\t\tdescription\n\n");

  for (i = 0; i < POCC_NB_OPTS; ++i)
    if (opts[i].short_opt != '\0')
      {
	if (opts[i].expect_arg)
	  printf ("-%c\t--%s <arg> \t%s\t%s\n",
		  opts[i].short_opt,
		  opts[i].long_opt,
		  opts[i].status,
		  opts[i].description);
	else
	  printf ("-%c\t--%s \t%s\t%s\n",
		  opts[i].short_opt,
		  opts[i].long_opt,
		  opts[i].status,
		  opts[i].description);
      }
    else
      {
	if (opts[i].expect_arg)
	  printf ("  \t--%s <arg> \t%s\t%s\n",
		  opts[i].long_opt,
		  opts[i].status,
		  opts[i].description);
	else
	  printf ("  \t--%s \t%s\t%s\n",
		  opts[i].long_opt,
		  opts[i].status,
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
	{
	  fprintf (stderr, "[PoCC] Error: Unable to open file %s\n",
		   argv[ret]);
	  pocc_usage ();
	}
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
      while (ii >= 0 && buffer[ii--] != '.')
	;
      buffer[++ii] = '\0';
      strcat (buffer, ".pocc.c");
      options->output_file_name = strdup (buffer);
    }
  if (opt_tab[POCC_OPT_OUTFILE_SCOP])
    {
      options->output_scoplib_file_name =
	XMALLOC(char, strlen(options->output_file_name) + 6);
      strcpy (options->output_scoplib_file_name,
	      options->output_file_name);
      strcat (options->output_scoplib_file_name, ".scop");
    }

  // Help, again.
  if (opt_tab[POCC_OPT_HELP])
    print_help ();
  // Version.
  if (opt_tab[POCC_OPT_VERSION])
    print_version ();

  // Parser options.
  if (opt_tab[POCC_OPT_BOUNDED_CTXT])
    options->clan_bounded_context = 1;
  if (opt_tab[POCC_OPT_DEFAULT_CTXT])
    options->set_default_parameter_values = 1;
  if (opt_tab[POCC_OPT_INSCOP_FAKEARRAY])
    options->inscop_fakepoccarray = 1;
  if (opt_tab[POCC_OPT_READ_SCOP_FILE])
    options->read_input_scop_file = 1;

  if (opt_tab[POCC_OPT_CLOOGIFY_SCHED])
    options->cloogify_schedules = 1;


  // Dependence analysis.
  if (opt_tab[POCC_OPT_NO_CANDL])
    options->candl_pass = 0;
  if (opt_tab[POCC_OPT_CANDL_DEP_ISL_SIMP])
    options->candl_deps_isl_simplify = 1;
  if (opt_tab[POCC_OPT_CANDL_DEP_PRUNE_DUPS])
    options->candl_deps_prune_transcover = 1;

  // Feature extraction.
  if (opt_tab[POCC_OPT_POLYFEAT])
    options->polyfeat = 1;
  if (opt_tab[POCC_OPT_POLYFEAT_RAR])
    options->polyfeat_rar = 1;

  // Trash.
  if (opt_tab[POCC_OPT_TRASH])
    options->trash = 1;
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
      else if (! strcmp(opt_tab[POCC_OPT_LETSEE_TRAVERSAL], "ga"))
	pocc_error("GA heuristics implementation are not publicly available\n");
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
	options->pluto_fuse = MAXIMAL_FUSE;
      else if (! strcmp(opt_tab[POCC_OPT_PLUTO_FUSE], "smartfuse"))
	options->pluto_fuse = SMART_FUSE;
      else if (! strcmp(opt_tab[POCC_OPT_PLUTO_FUSE], "nofuse"))
	options->pluto_fuse = NO_FUSE;
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
  if (opt_tab[POCC_OPT_PLUTO_RAR_CF])
    options->pluto_rar_cf = options->pluto = 1;
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
      options->pluto_ft = atoi (opt_tab[POCC_OPT_PLUTO_LT]);
      options->pluto = 1;
    }
  if (opt_tab[POCC_OPT_PLUTO_EXTERNAL_CANDL])
    {
      options->pluto_external_candl = 1;
      options->pluto = 1;
    }
  if (opt_tab[POCC_OPT_PLUTO_TILING_IN_SCATT])
    {
      options->pluto_tiling_in_scatt = 1;
      options->pluto = 1;
    }
  if (opt_tab[POCC_OPT_PLUTO_BOUND_COEF])
    {
      options->pluto_bound_coefficients =
	atoi (opt_tab[POCC_OPT_PLUTO_BOUND_COEF]);
      options->pluto = 1;
    }
  if (opt_tab[POCC_OPT_LETSEE_BOUNDS])
    {
      char buff[16];
      char* str = opt_tab[POCC_OPT_LETSEE_BOUNDS];
      int pos = 0;
      int count = 0;
#define dirty_parse(val, check)					\
      count = 0;						\
      while (str[pos] && str[pos] != ',')			\
	buff[count++] = str[pos++];				\
      if (check && ! str[pos])					\
	pocc_error ("Missing argument in --letsee-bounds\n");	\
      buff[count] = '\0';					\
      val = atoi (buff);					\
      ++pos;
      dirty_parse(options->letsee_ilb, 1);
      dirty_parse(options->letsee_iUb, 1);
      dirty_parse(options->letsee_plb, 1);
      dirty_parse(options->letsee_pUb, 1);
      dirty_parse(options->letsee_clb, 1);
      dirty_parse(options->letsee_cUb, 0);
    }

  // Compile.
  if (options->letsee || opt_tab[POCC_OPT_COMPILE])
    options->compile_program = 1;
  // Compile command.
  if (opt_tab[POCC_OPT_COMPILE_CMD])
    {
      options->compile_command = strdup (opt_tab[POCC_OPT_COMPILE_CMD]);
      options->compile_program = 1;
    }
  else
    options->compile_command = strdup ("gcc -O3 -lm");
  // Execution command arguments.
  if (opt_tab[POCC_OPT_RUN_CMD_ARGS])
    options->execute_command_args = strdup (opt_tab[POCC_OPT_RUN_CMD_ARGS]);

  if (opt_tab[POCC_OPT_LETSEE_DRY_RUN])
    {
      options->compile_program = 0;
      options->letsee = 1;
    }
  // Timeout
  if (opt_tab[POCC_OPT_PROGRAM_TIMEOUT])
    options->timeout = atoi (opt_tab[POCC_OPT_PROGRAM_TIMEOUT]);

  // Codegen/Cloog options.
  if (opt_tab[POCC_OPT_NOCODEGEN])
    {
      options->codegen = 0;
      options->compile_program = 0;
    }
  if (opt_tab[POCC_OPT_CLOOG_F])
    options->cloog_f = atoi (opt_tab[POCC_OPT_CLOOG_F]);
  if (opt_tab[POCC_OPT_CLOOG_L])
    options->cloog_l = atoi (opt_tab[POCC_OPT_CLOOG_L]);
  if (opt_tab[POCC_OPT_CODEGEN_TIMERCODE])
    options->codegen_timercode = 1;
  if (opt_tab[POCC_OPT_CODEGEN_TIMER_ASM])
    options->codegen_timer_asm = 1;
  if (opt_tab[POCC_OPT_CODEGEN_TIMER_PAPI])
    options->codegen_timer_papi = 1;
  if (opt_tab[POCC_OPT_PRINT_CLOOG_FILE])
    options->print_cloog_file = 1;

  // PAST options.
  if (opt_tab[POCC_OPT_NO_PAST])
    options->use_past = 0;
  if (opt_tab[POCC_OPT_PAST_OPTIMIZE_LOOP_BOUND])
    options->past_optimize_loop_bounds = 1;

    // Pragmatizer options.
  if (opt_tab[POCC_OPT_PRAGMATIZER])
    options->pragmatizer = 1;

  // PTile options.
  if (opt_tab[POCC_OPT_PTILE])
    {
      options->ptile = 1;
      options->use_past = 1;
    }
  // PTile options.
  if (opt_tab[POCC_OPT_PTILE_FTS])
    {
      options->ptile_fts = 1;
      options->ptile = 1;
      options->use_past = 1;
    }

  // PUnroller options.
  if (opt_tab[POCC_OPT_PUNROLL])
    {
      options->punroll = 1;
      options->use_past = 1;
    }
  if (opt_tab[POCC_OPT_PUNROLL_AND_JAM])
    {
      options->punroll = 1;
      options->punroll_and_jam = 1;
      options->use_past = 1;
    }
  if (opt_tab[POCC_OPT_PUNROLL_SIZE])
    {
      options->punroll = 1;
      options->punroll_size = atoi (opt_tab[POCC_OPT_PUNROLL_SIZE]);
      options->use_past = 1;
    }

#ifndef POCC_RELEASE_MODE

  // Vectorizer options.
  if (opt_tab[POCC_OPT_VECTORIZER])
    options->vectorizer = options->vectorizer_vectorize_loops = 1;
  if (opt_tab[POCC_OPT_VECT_MARK_PAR_LOOPS])
    options->vectorizer_mark_par_loops = 1;
  if (opt_tab[POCC_OPT_VECT_NO_KEEP_OUTER_PAR_LOOPS])
    options->vectorizer_keep_outer_par_loops = 0;
  if (opt_tab[POCC_OPT_VECT_SINK_ALL_LOOPS])
    options->vectorizer_sink_all_candidates = 1;

  // Storage compaction options.
  if (opt_tab[POCC_OPT_STORCOMPACT])
    options->storage_compaction = 1;
  if (opt_tab[POCC_OPT_AC_KEEP_OUTERPAR])
    options->array_contraction_keep_outer_par_loops = 1;
  if (opt_tab[POCC_OPT_AC_KEEP_VECTORIZED])
    options->array_contraction_keep_vectorized_loops = 1;



  // Ponos options.
  if (opt_tab[POCC_OPT_PONOS])
    options->ponos = 1;
  if (opt_tab[POCC_OPT_PONOS_QUIET])
    options->ponos_quiet = 1;
  if (opt_tab[POCC_OPT_PONOS_DEBUG])
    options->ponos_debug = 1;
  if (opt_tab[POCC_OPT_PONOS_SCHED_DIMENSION])
    options->schedule_dim = atoi (opt_tab[POCC_OPT_PONOS_SCHED_DIMENSION]);
  if (opt_tab[POCC_OPT_PONOS_COEF_ARE_POS])
    options->ponos_coef_are_pos = 1;
  if (opt_tab[POCC_OPT_PONOS_BUILD_2DP1])
    options->ponos_build_2d_plus_one = 1;
  if (opt_tab[POCC_OPT_PONOS_SOLVER_TYPE])
    {
      if (! strcmp(opt_tab[POCC_OPT_PONOS_SOLVER_TYPE], "pip"))
	// hardwire '1', until ponos gets in stable pocc mode.
	options->ponos_solver_type = 1;
    }
  if (opt_tab[POCC_OPT_PONOS_MAXSCALE_SOLVER])
    options->ponos_maxscale_solver = 1;
  if (opt_tab[POCC_OPT_PONOS_NOREDUNDANCY_SOLVER])
    options->ponos_noredundancy_solver = 1;
  if (opt_tab[POCC_OPT_PONOS_LEGALITY_CONSTANT_K])
    options->ponos_legality_constant_K =
      atoi (opt_tab[POCC_OPT_PONOS_LEGALITY_CONSTANT_K]);
  if (opt_tab[POCC_OPT_PONOS_SCHED_COEF_BOUND])
    options->ponos_schedule_bound =
      atoi (opt_tab[POCC_OPT_PONOS_SCHED_COEF_BOUND]);

  // Past options.
  if (opt_tab[POCC_OPT_PAST_SUPER_OPT_LOOP_BOUND])
    options->past_super_optimize_loop_bounds = 1;


#endif
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
