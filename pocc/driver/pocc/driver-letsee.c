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
pocc_driver_after_letsee (s_pocc_utils_options_t* puoptions)
{
  s_pocc_options_t* poptions = puoptions->pocc_options;
  // Backup the input scop.
  clan_scop_p input_scop = clan_scop_dup (puoptions->program);

  // Run PLuTo, if required.
  if (poptions->pluto || poptions->letsee_space == LS_TYPE_FS)
      pocc_driver_pluto (puoptions->program, poptions, puoptions);

  // Generate the code, if required.
  if (poptions->codegen)
    {
      char* backup_output_file_name = poptions->output_file_name;
      poptions->output_file_name = puoptions->output_file_name;
      poptions->compile_program = 1;
      poptions->execute_program = 1;
      pocc_driver_codegen (puoptions->program, poptions, puoptions);
      poptions->output_file_name = backup_output_file_name;
    }
  puoptions->program_exec_result = poptions->program_exec_result;

  // Restore the input scop.
  clan_scop_free (puoptions->program);
  puoptions->program = input_scop;
}

void 
pocc_driver_letsee (clan_scop_p program, 
		    s_pocc_options_t* poptions,
		    s_pocc_utils_options_t* puoptions)
{
  printf ("[PoCC] Running LetSee\n");
  s_ls_options_t* loptions = ls_options_malloc ();

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

  letsee_pocc (cprogram, loptions, puoptions);

  ls_options_free (loptions);
}
