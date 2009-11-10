/*
 * exec.c: this file is part of the PoCC project.
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
#include <pocc/exec.h>


/**
 * Execute a command line (supplied by args) and return a string from
 * its standard output (supposed to be the metric, eg, cycles).
 *
 */
static
char*
pocc_execprog_ (char** args, int return_result, int show_output, int noexit)
{
  pid_t pid;
  int rv;
  int commpipe[2];
  char buf[32000];
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
      for (i = 0; i < 32000; ++i)
	buf[i] = '\0';
      while (read (0, buf, 32000))
	if (show_output)
	  printf ("%s", buf);
      wait (&rv);
      if (rv != 0)
	{
	  char* err_type;
	  if (noexit)
	    err_type = "WARNING:";
	  else
	    err_type = "FATAL:";
	  fprintf (stderr, "[PoCC] %s error executing ", err_type);
	  for (i = 0; args[i]; ++i)
	    fprintf (stderr, "%s ", args[i]);
	  fprintf (stderr, "\n");
	  if (strlen (buf) != 0)
	    fprintf (stderr, "[PoCC] %s execv output: %s\n", err_type, buf);
	  else
	    fprintf (stderr, "[PoCC] %s command is not executable\n",
		     err_type);
	  fprintf (stderr, "[PoCC] %s execv exit status: %d\n", err_type, rv);
	  if (! noexit)
	    exit (rv);
	  else
	    return NULL;
	}
      close (commpipe[0]);
    }
  else
    {
      // Child.
      dup2 (commpipe[1], 1);
      dup2 (commpipe[1], 2);
      close (commpipe[0]);
      if (execvp (args[0], args) == -1)
	{
	  // fprintf (stderr, "execv Error.\n");
	  exit (1);
	}
      close (commpipe[1]);
    }

  if (return_result)
    return strdup (buf);
  return NULL;
}


void
pocc_exec (char** args, int show_output)
{
  pocc_execprog_ (args, 0, show_output, 0);
}


char*
pocc_exec_string (char** args, int show_output)
{
  return pocc_execprog_ (args, 1, show_output, 0);
}


char*
pocc_exec_string_noexit (char** args, int show_output)
{
  return pocc_execprog_ (args, 1, show_output, 1);
}
