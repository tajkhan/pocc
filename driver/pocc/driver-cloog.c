/*
 * driver-cloog.c: this file is part of the PoCC project.
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
#if HAVE_CONFIG_H
# include <pocc-utils/config.h>
#endif

# include <pocc/driver-cloog.h>



static
CloogMatrix* convert_to_cloogmatrix(scoplib_matrix_p mat)
{
  CloogMatrix* ret = cloog_matrix_alloc (mat->NbRows, mat->NbColumns);

  int i, j;
  for (i = 0; i < mat->NbRows; ++i)
    for (j = 0; j < mat->NbColumns; ++j)
      cloog_int_set_si(ret->p[i][j], SCOPVAL_get_si(mat->p[i][j]));

  return ret;
}

/**
 * Create a CLooG schedule from a pluto scattering.
 * What a mess for tiling into the scattering!
 *
 */
static
scoplib_matrix_p cloogify_schedule(scoplib_matrix_p mat,
				   int nb_scatt, int nb_par)
{
  int i, j, k, l;
  int nb_ineq;
  int nb_eq;

  // Count the number of inequalities. Foolishly assume two
  // inequalities per tloop...
  for (i = 0, nb_ineq = 0, nb_eq = 0; i < mat->NbRows; ++i)
    {
      if (SCOPVAL_get_si(mat->p[i][0]) == 1)
	++nb_ineq;
      else
	++nb_eq;
    }
  int nb_tile_loops = nb_ineq / 2;

  // Allocate new schedule. 'mat' already contains extra columns for
  // the tloop.
  scoplib_matrix_p ret =
    scoplib_matrix_malloc (nb_scatt + nb_ineq, mat->NbColumns + nb_scatt);

  int scaldim_offset = 0;
  int line = 0;
  int last_offset;
  for (i = 0; i < mat->NbRows; ++i)
    {
      SCOPVAL_set_si(ret->p[i][0], SCOPVAL_get_si(mat->p[i][0]));
      if (SCOPVAL_get_si(mat->p[i][0]) == 0)
	{
	  // Row in the scheduling matrix is an equality. Copy it.
	  SCOPVAL_set_si(ret->p[i][i + 1 + nb_tile_loops], -1);
	  for (j = 1; j < mat->NbColumns; ++j)
	    SCOPVAL_set_si(ret->p[i][j + nb_scatt],
			   SCOPVAL_get_si(mat->p[i][j]));
	}
      else
	{
	  // Row is an inequality for tiling. Recover tloop components.

	  // Compute the scalar offset of this tloop level, ie, how
	  // many scalar dimension before the tloop_level tloop.
	  do
	    {
	      last_offset = scaldim_offset;
	      for (j = 1; line < mat->NbRows && j < mat->NbColumns - 1 &&
		     SCOPVAL_get_si(mat->p[line][j]) == 0; ++j)
		;
	      if (j == mat->NbColumns - 1)
		++scaldim_offset;
	      ++line;
	    }
	  while (scaldim_offset > last_offset && line < mat->NbRows);

	  // Copy the tloop-related section
	  for (j = 1; j <= nb_tile_loops; ++j)
	    SCOPVAL_set_si(ret->p[i][j],
			   SCOPVAL_get_si(mat->p[i][j]));
	  // Copy the ploop-related section
	  for (; j < mat->NbColumns - nb_par - 1; ++j)
	    SCOPVAL_set_si(ret->p[i][j + nb_tile_loops + scaldim_offset],
			   SCOPVAL_get_si(mat->p[i][j]));
	  // Copy the parametric and scalar section
	  for (; j < mat->NbColumns; ++j)
	    SCOPVAL_set_si(ret->p[i][j + nb_scatt],
			   SCOPVAL_get_si(mat->p[i][j]));
	}
    }

  // Complete the schedule to have unspecified scattering dimensions
  // set to 0.
  for (; i < ret->NbRows; ++i)
    SCOPVAL_set_si(ret->p[i][i + 1 - nb_ineq], -1);

  // Associate tloop bounds with tloop schedule. Overwrite the
  // previous completion if necessary.
  scaldim_offset = 0;
  line = 0;
  for (k = 0; k < nb_tile_loops; ++k)
    {
      do
	{
	  last_offset = scaldim_offset;
	  for (j = 1; line < mat->NbRows && j < mat->NbColumns - 1 &&
		 SCOPVAL_get_si(mat->p[line][j]) == 0; ++j)
	    ;
	  if (j == mat->NbColumns - 1)
	    ++scaldim_offset;
	  ++line;
	}
      while (scaldim_offset > last_offset && line < mat->NbRows);

      // Clear the dimension.
      for (j = 0; j < ret->NbColumns; ++j)
	SCOPVAL_set_si(ret->p[ret->NbRows - nb_tile_loops + k][j], 0);
      // Set the tloop bound <-> tloop schedule association.
      SCOPVAL_set_si(ret->p[ret->NbRows - nb_tile_loops + k][k + 1], -1);
      SCOPVAL_set_si(ret->p[ret->NbRows - nb_tile_loops + k]
		     [k + 1 + nb_tile_loops + scaldim_offset],  1);
    }

  // Re-interleave tloop bounds dimensions with scalar dimensions.
  int placed = 0;
  line = 0;
  for (k = 0; k < nb_tile_loops; ++k)
    {
      scaldim_offset = 0;
      do
	{
	  last_offset = scaldim_offset;
	  for (j = 1; line < mat->NbRows && j < mat->NbColumns - 1 &&
		 SCOPVAL_get_si(mat->p[line][j]) == 0; ++j)
	    ;
	  if (j == mat->NbColumns - 1)
	    ++scaldim_offset;
	  ++line;
	}
      while (scaldim_offset > last_offset && line < mat->NbRows);

      // Put the scaldim_offset scalar dimensions before the k tloop
      // bound dimension.
      for (i = 0; i < scaldim_offset; ++i)
	{
	  // First scalar loop is '1 + nb_tile_loops', we are at loop 'i',
	  // and we already placed 'placed' loops.
	  for (j = 1 + nb_tile_loops + i + placed - 1;
	       j >= 1 + k + i + placed; --j)
	    {
	      // permute_column (ret, j, j + 1);
	      for (l = 0; l < ret->NbRows; ++l)
		{
		  int tmp = SCOPVAL_get_si(ret->p[l][j]);
		  SCOPVAL_set_si(ret->p[l][j], ret->p[l][j + 1]);
		  SCOPVAL_set_si(ret->p[l][j + 1], tmp);
		}
	    }
	}
      placed += scaldim_offset + 1;
    }

  return ret;
}


/**
 * Create a complete, CLooG-compatible scattering from the
 * Pluto/LetSee generated scatterings.
 *
 * This is needed in particular when tiling constraints are to be
 * embedded in the scatterings.
 *
 */
void pocc_cloogify_scop(scoplib_scop_p program)
{
  int i;
  scoplib_statement_p stm;
  scoplib_matrix_p mat;

  // Collect the maximal scattering dimensionality.
  int nb_scatt = 0;
  for (stm = program->statement; stm; stm = stm->next)
    {
      int nb_eq = 0;
      for (i = 0; i < stm->schedule->NbRows; ++i)
	if (SCOPVAL_get_si(stm->schedule->p[i][0]) == 0)
	  ++nb_eq;
      int cur_scatt = nb_eq + (stm->schedule->NbColumns -
			       stm->domain->elt->NbColumns);
      nb_scatt = nb_scatt > cur_scatt ? nb_scatt : cur_scatt;
    }

  for (stm = program->statement; stm; stm = stm->next)
    {
      scoplib_matrix_p newsched =
	cloogify_schedule ((scoplib_matrix_p) stm->schedule, nb_scatt,
			   program->nb_parameters);
      scoplib_matrix_free (stm->schedule);
      stm->schedule = newsched;
    }
}


/**
 *
 * Call CLooG, return a CLAST. Supports tiling-in-scattering, from
 * PluTo.
 *
 *
 */
struct clast_stmt*
pocc_driver_cloog (scoplib_scop_p program,
		   CloogOptions* coptions,
		   s_pocc_options_t* poptions,
		   s_pocc_utils_options_t* puoptions)
{
  CloogMatrix* cmat;
  CloogInput* input;
  struct clast_stmt* root;

  // Convert context.
  cmat = convert_to_cloogmatrix (program->context);
  CloogDomain* context = cloog_domain_from_cloog_matrix (coptions->state, cmat,
							program->nb_parameters);
  cloog_matrix_free (cmat);

  // Convert statements.
  CloogUnionDomain* ud = cloog_union_domain_alloc (program->nb_parameters);

  // Update parameter names.
  int i;
  for (i = 0; i < program->nb_parameters; ++i)
    ud = cloog_union_domain_set_name (ud, CLOOG_PARAM, i,
				      program->parameters[i]);

  // Iterate on statements, and create one per component of the union
  // in a domain.
  scoplib_statement_p stm;
  CloogDomain* dom;
  CloogScattering* scat;
  scoplib_matrix_p mat;
  char** names;
  int nb_tile_loops;

  // Collect the maximal scattering dimensionality.
  int nb_scatt = 0;
  for (stm = program->statement; stm; stm = stm->next)
    {
      int nb_eq = 0;
      for (i = 0; i < stm->schedule->NbRows; ++i)
	if (SCOPVAL_get_si(stm->schedule->p[i][0]) == 0)
	  ++nb_eq;
      int cur_scatt = nb_eq + (stm->schedule->NbColumns -
			       stm->domain->elt->NbColumns);
      nb_scatt = nb_scatt > cur_scatt ? nb_scatt : cur_scatt;
    }

  for (stm = program->statement; stm; stm = stm->next)
    {
      scoplib_matrix_list_p smat;
      for (smat = stm->domain; smat; smat = smat->next)
	{
	  mat = (scoplib_matrix_p) smat->elt;
	  cmat = convert_to_cloogmatrix (mat);
	  dom = cloog_domain_from_cloog_matrix (coptions->state, cmat,
						program->nb_parameters);
	  cloog_matrix_free (cmat);
	  scoplib_matrix_p newsched =
	    cloogify_schedule ((scoplib_matrix_p) stm->schedule, nb_scatt,
			       program->nb_parameters);
	  int total_scatt_dims = newsched->NbColumns - mat->NbColumns;
	  cmat = convert_to_cloogmatrix (newsched);
	  scoplib_matrix_free (newsched);
	  scat = cloog_scattering_from_cloog_matrix (coptions->state, cmat,
						     total_scatt_dims,
						     program->nb_parameters);
	  ud = cloog_union_domain_add_domain (ud, NULL, dom, scat, NULL);
	  cloog_matrix_free (cmat);
	}
    }

  input = cloog_input_alloc (context, ud);

  // Generate the clast.
  root = cloog_clast_create_from_input (input, coptions);

  /// FIXME: reactivate this. input must be passed
  //cloog_input_free (input);

  return root;
}
