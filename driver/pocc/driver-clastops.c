/*
 * driver-clastops.c: this file is part of the PoCC project.
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

# define CLOOG_SUPPORTS_SCOPLIB
# include <cloog/cloog.h>
# include <cloog/clast.h>
# include <pragmatize/pragmatize.h>
# include <vectorizer/vectorizer.h>
# include <pocc/driver-clastops.h>


void
pocc_driver_clastops (scoplib_scop_p program,
		      CloogProgram* cp,
		      s_pocc_options_t* poptions,
		      s_pocc_utils_options_t* puoptions)
{
  CloogOptions* coptions = poptions->cloog_options;

  // Create the CLAST associated to the CloogProgram.
  struct clast_stmt* root = cloog_clast_create (cp, coptions);

  // Run the vectorizer, if required.
  if (poptions->vectorizer)
    {
      if (! poptions->quiet)
	printf ("[PoCC] Running vectorizer\n");
      // Call the vectorizer.
      vectorizer (program, root);
    }

  // Run the pragmatizer, if required.
  if (poptions->pragmatizer)
    {
      if (! poptions->quiet)
	printf ("[PoCC] Running pragmatize\n");
      pragmatize (program, root);
    }

  // Run the extended CLAST pretty-printer
  if (poptions->pragmatizer || poptions->vectorizer)
    pragmatize_clast_pprint (poptions->output_file, root, 0, coptions);
  else
    // Pretty-print the code with CLooG default pretty-printer.
    clast_pprint (poptions->output_file, root, 0, coptions);

  // Delete the clast.
  cloog_clast_free (root);
}
