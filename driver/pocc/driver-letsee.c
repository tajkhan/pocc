/*
 * driver-letsee.c: this file is part of the PoCC project.
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

# include <pocc/driver-letsee.h>
# include <letsee/pocc-driver.h>


void
pocc_copy_transfo_to_scop (s_pocc_utils_options_t* puoptions,
			   scoplib_scop_p scop)
{
  s_fm_vector_t** transfo = puoptions->transfo_matrices;
  scoplib_statement_p stm;
  int nb_par = scop->context->NbColumns - 2;
  int dim;
  int count = 0;
  int iter_pos = 1;
  int par_pos = 1;
  int cst_pos = 0;
  int i, j;

  for (dim = 0; transfo[dim]; ++dim)
    ;

  for (stm = scop->statement; stm; stm = stm->next)
    {
      par_pos += stm->nb_iterators;
      cst_pos += nb_par;
    }
  cst_pos += par_pos;
  for (stm = scop->statement; stm; stm = stm->next)
    {
      scoplib_matrix_free (stm->schedule);
      stm->schedule =
	scoplib_matrix_malloc (dim, 1 + stm->nb_iterators + nb_par + 1);
      for (i = 0, count = 1; i < dim; ++i)
	{
	  for (j = 0; j < stm->nb_iterators; ++j)
	    SCOPVAL_set_si(stm->schedule->p[i][count++],
			   transfo[i]->vector[j + iter_pos].num);
	  for (j = 0; j < nb_par; ++j)
	    SCOPVAL_set_si(stm->schedule->p[i][count++],
			   transfo[i]->vector[j + par_pos].num);
	  SCOPVAL_set_si(stm->schedule->p[i][count++],
			 transfo[i]->vector[cst_pos].num);
	}
      iter_pos += stm->nb_iterators;
      par_pos += nb_par;
      cst_pos++;
    }
}



void
pocc_driver_after_letsee (s_pocc_utils_options_t* puoptions)
{
  s_pocc_options_t* poptions = puoptions->pocc_options;
  // Backup the input scop.
  scoplib_scop_p input_scop = scoplib_scop_dup (puoptions->program);

  // Run PLuTo, if required.
  if (poptions->letsee_space == LS_TYPE_FS)
    if (pocc_driver_pluto (puoptions->program, poptions, puoptions) ==
	EXIT_FAILURE)
      {
	if (! poptions->quiet)
	  printf ("[PoCC] Error in performing PLuTo. Optimization aborted\n");
	return;
      }
  if (poptions->letsee_space == LS_TYPE_MULTI)
    pocc_copy_transfo_to_scop (puoptions, puoptions->program);

  // Generate the code, if required.
  if (poptions->codegen)
    {
      char* backup_output_file_name = poptions->output_file_name;
      poptions->output_file_name = puoptions->output_file_name;
      if (poptions->compile_program)
	poptions->execute_program = 1;
      pocc_driver_codegen (puoptions->program, poptions, puoptions);
      poptions->output_file_name = backup_output_file_name;
    }
  puoptions->program_exec_result = poptions->program_exec_result;
  // Restore the input scop.
  scoplib_scop_free (puoptions->program);
  puoptions->program = input_scop;
}

void
pocc_driver_letsee (scoplib_scop_p program,
		    s_pocc_options_t* poptions,
		    s_pocc_utils_options_t* puoptions)
{
  printf ("[PoCC] Running LetSee\n");
  s_ls_options_t* loptions = ls_options_malloc ();

  if (poptions->letsee_space == LS_TYPE_MULTI)
    {
      loptions->normalize_space = 1;
      loptions->noredundancy_solver = 1;
    }
  loptions->type = poptions->letsee_space;
  loptions->create_schedfiles = poptions->codegen;
  XFREE(loptions->transfo_dir);
  loptions->transfo_dir = XMALLOC(char, 8192);
  strcpy (loptions->transfo_dir, poptions->output_file_name);
  loptions->transfo_dir[strlen (loptions->transfo_dir) - 2] = '\0';
  strcat (loptions->transfo_dir, "-letsee-transformations");
  loptions->heuristic = poptions->letsee_traversal;
  loptions->scheme_m1 = poptions->letsee_scheme_m1;
  loptions->backtrack_mode = poptions->letsee_backtrack_multi;
  loptions->prune_oset = poptions->letsee_prune_precut;
  loptions->normalize_space = poptions->letsee_normspace;
  //loptions->thresold = poptions->letsee_thresold;
  loptions->rtries = poptions->letsee_rtries;
  loptions->verbose = poptions->verbose;
  // Convert the scop to a candl program (LetSee IR).
  candl_program_p cprogram = candl_program_convert_scop(program, NULL);
  // Store the .scop in the pocc-utils wrapper.
  puoptions->program = program;
  puoptions->pocc_options = poptions;
  puoptions->pocc_codegen = pocc_driver_after_letsee;
  puoptions->input_file_name = poptions->input_file_name;
  puoptions->output_file_name = NULL;
  // Open the iterative.dat file.
  if (poptions->compile_program)
    {
      puoptions->data_file = fopen ("iterative.dat", "w+");
      if (puoptions->data_file == NULL)
	pocc_error ("Cannot create file iterative.dat");
      fprintf (puoptions->data_file, "# LetSee results for %s\n",
	       puoptions->input_file_name);
    }
  // Run LetSee.
  letsee_pocc (cprogram, loptions, puoptions);
  if (poptions->compile_program)
    printf ("[PoCC] Iterative results stored in file iterative.dat\n");
  // Be clean.
  if (poptions->compile_program)
    fclose (puoptions->data_file);
  ls_options_free (loptions);
}
