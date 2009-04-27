/*
 * driver-codegen.c: this file is part of the PoCC project.
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

# include <pocc/driver-codegen.h>
# define TO_STRING__(x) #x
# define TO_STRING_(x) TO_STRING__(x)
# define STR_POCC_ROOT_DIR TO_STRING_(POCC_ROOT_DIR)


/**
 * Execute a command line (supplied by args) and return a long long
 * int read from its standard output (supposed to be the metric, eg,
 * cycles).
 *
 * Dedicated to execute a transformation.
 *
 */
static
void
pocc_execprog (char** args)
{
  pid_t pid;
  int rv;
  int commpipe[2];
  char buf[1024];
  int i;

  if (pipe (commpipe))
    {
      fprintf (stderr, "Pipe error.\n");
      exit (1);
    }

  if((pid = fork ()) == -1)
    {
      fprintf (stderr, "Fork error.\n");
      exit (1);
    }

  if (pid)
    {
      // Parent.
      dup2 (commpipe[0], 0);
      close (commpipe[1]);
      for (i = 0; i < 1024; ++i)
	buf[i] = '\0';
      while (read (0, buf, 1024))
	;
      wait (&rv);
      if (rv != 0)
	{
	  printf ("exit status: %d\n", rv);
	  printf ("output: %s\n", buf);
	}
      close (commpipe[0]);
    }
  else
    {
      // Child.
      dup2 (commpipe[1], 1);
      close (commpipe[0]);
      if (execvp (args[0], args) == -1)
	{
	  fprintf (stderr, "execv Error.\n");
	  exit (1);
	}
      close (commpipe[1]);
    }
}


void
pocc_driver_codegen_post_processing (FILE* body_file, 
				     s_pocc_options_t* poptions)
{
  char* args[4];
  args[2] = args[3] = NULL;
  args[1] = ".body.c";
  if (poptions->pluto_parallel) 
    {
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/ploog";
      pocc_execprog (args);
    }
  if (poptions->pluto_unroll) 
    {
      // Run plann.
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/plann";
      args[2] = STR_POCC_ROOT_DIR "/generators/scripts/annotations";
      pocc_execprog (args);
      args[2] = NULL;
    }

  if (poptions->pluto_prevector)
    {
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/vloog";
      pocc_execprog (args);
    }
}

void
pocc_driver_codegen_program_finalize (s_pocc_options_t* poptions)
{
  char* args[4];
  args[3] = NULL;
  args[0] = STR_POCC_ROOT_DIR "/generators/scripts/inscop";
  args[1] = poptions->input_file_name;
  args[2] = ".body.c";
  args[3] = poptions->output_file_name;
  pocc_execprog (args);
  if (poptions->codegen_timercode)
    {
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/timercode";
      args[1] = poptions->output_file_name;
      pocc_execprog (args);
    }

  if (poptions->pluto_parallel)
    {
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/omp";
      args[1] = poptions->output_file_name;
      pocc_execprog (args);
    }
}


void 
pocc_driver_codegen (clan_scop_p program, 
		     s_pocc_options_t* poptions,
		     s_pocc_utils_options_t* puoptions)
{
  printf ("[PoCC] Running Codegen...\n");
  // Backup the default output file.
  FILE* out_file = poptions->output_file;
  FILE* body_file = fopen (".body.c", "w+");
  if (body_file == NULL)
    pocc_error ("Cannot create file .body.c\n");

  // Generate kernel code.
  poptions->output_file = body_file;
  pocc_driver_cloog (program, poptions, puoptions);
  fclose (poptions->output_file);
  // Perform syntactic post-processing.
  pocc_driver_codegen_post_processing (body_file, poptions);
  // Build the final output file.
  pocc_driver_codegen_program_finalize (poptions);
  // Restore the default output file.
  poptions->output_file = out_file;
}



void* 
pocc_codegen (void* program, 
	      s_pocc_utils_options_t* puoptions)
{
  
}
