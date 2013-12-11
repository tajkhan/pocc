/*
 * driver-codegen.c: this file is part of the PoCC project.
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

# ifndef CLOOG_INT_GMP
#  define CLOOG_INT_GMP
# endif
# include <osl/scop.h>
# include <osl/extensions/loop.h>
# include <osl/extensions/coordinates.h>

# include <cloog/cloog.h>
# include <pocc/driver-codegen.h>
# include <pocc/driver-cloog.h>
# include <pocc/driver-clastops.h>
# include <pocc/driver-pastops.h>
# include <clasttools/clast2past.h>

static
void
pocc_driver_codegen_post_processing (FILE* body_file,
				     s_pocc_options_t* poptions)
{
  char* args[4];
  args[2] = args[3] = NULL;
  args[1] = ".body.c";
  if (poptions->pluto_parallel && ! poptions->pragmatizer &&
      ! poptions->use_past && ! poptions->ptile)
    {
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/ploog";
      if (poptions->quiet)
	pocc_exec (args, POCC_EXECV_HIDE_OUTPUT);
      else
	pocc_exec (args, POCC_EXECV_SHOW_OUTPUT);
    }
  if (poptions->pluto_unroll)
    {
      // Run plann.
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/plann";
      args[2] = STR_POCC_ROOT_DIR "/generators/scripts/annotations";
      if (poptions->quiet)
	pocc_exec (args, POCC_EXECV_HIDE_OUTPUT);
      else
	pocc_exec (args, POCC_EXECV_SHOW_OUTPUT);
      args[2] = NULL;
    }
  if (poptions->pluto_prevector && ! poptions->pragmatizer &&
      ! poptions->use_past && ! poptions->ptile)
    {
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/vloog";
      if (poptions->quiet)
	pocc_exec (args, POCC_EXECV_HIDE_OUTPUT);
      else
	pocc_exec (args, POCC_EXECV_SHOW_OUTPUT);
    }
}


static
int
pocc_driver_codegen_program_finalize (s_pocc_options_t* poptions)
{
  char* args[10];
  if (poptions->inscop_fakepoccarray == 1)
    args[0] = STR_POCC_ROOT_DIR "/generators/scripts/inscop-fakearray";
  else
    args[0] = STR_POCC_ROOT_DIR "/generators/scripts/inscop";
  args[1] = poptions->input_file_name;
  args[2] = ".body.c";
  args[3] = poptions->output_file_name;
  args[4] = args[5] = args[6] = args[7] = args[8] = NULL;
  int mode = poptions->quiet ? POCC_EXECV_HIDE_OUTPUT : POCC_EXECV_SHOW_OUTPUT;
  pocc_exec (args, mode);
  if (poptions->codegen_timercode)
    {
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/timercode";
      args[1] = poptions->output_file_name;
      args[2] = "time";
      args[3] = NULL;
      pocc_exec (args, mode);
    }
  else if (poptions->codegen_timer_asm)
    {
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/timercode";
      args[1] = poptions->output_file_name;
      args[2] = "asm";
      args[3] = NULL;
      pocc_exec (args, mode);
    }

  if (poptions->pluto_parallel)
    {
      args[0] = STR_POCC_ROOT_DIR "/generators/scripts/omp";
      args[1] = poptions->output_file_name;
      args[2] = NULL;
      pocc_exec (args, mode);
    }

  // Compile the program, if necessary.
  int compile_success = 0;
  if (poptions->compile_program)
    {
      int offset = 0;
      char buffer[8192];
      if (poptions->timeout > 0)
	{
	  args[0] = "perl";
	  args[1] = "-e";
	  args[2] = "alarm shift @ARGV; exec @ARGV";
	  sprintf (buffer, "%d", poptions->timeout);
	  args[3] = strdup (buffer);
	  offset = 4;
	}
      args[offset] = STR_POCC_ROOT_DIR "/generators/scripts/compile";
      args[offset + 1] = poptions->output_file_name;
      strcpy (buffer, poptions->compile_command);
      strcat (buffer, " -lm");
      if (poptions->codegen_timer_asm || poptions->codegen_timercode)
	strcat (buffer, " -DTIME");
      args[offset + 2] = buffer;
      args[offset + 3] = XMALLOC(char, strlen (poptions->output_file_name) + 2);
      strcpy (args[offset + 3], poptions->output_file_name);
      // Remove the .xxx extension, if any.
      int pos = strlen (args[offset + 3]) - 1;
      while (args[offset + 3][pos] != '.' && --pos)
	;
      if (pos != 0)
	args[offset + 3][pos] = '\0';
      args[offset + 4] = NULL;

      char* res = pocc_exec_string_noexit (args, mode);
      if (res != NULL)
	{
	  compile_success = 1;
	  XFREE(res);
	}
      else
	return EXIT_FAILURE;
    }

  // Run the program, if necessary.
  if (poptions->compile_program && poptions->execute_program && compile_success)
    {
      int offset = 0;
      char buffer[8192];
      if (poptions->timeout > 0)
	{
	  args[0] = "perl";
	  args[1] = "-e";
	  args[2] = "alarm shift @ARGV; exec @ARGV";
	  sprintf (buffer, "%d", poptions->timeout);
	  args[3] = strdup (buffer);
	  offset = 4;
	}
      args[offset] = STR_POCC_ROOT_DIR "/generators/scripts/execute";
      ++offset;
      int len = strlen (poptions->output_file_name) + 5;
      if (poptions->execute_command_args != NULL)
	len += strlen (poptions->execute_command_args);
      args[offset] = XMALLOC(char, len);
      strcpy (args[offset], "./");
      strcat (args[offset], poptions->output_file_name);
      int pos = strlen (args[offset]) - 1;
      while (args[offset][pos] != '.' && --pos)
	;
      if (pos != 0)
	args[offset][pos] = '\0';
      strcat (args[offset], " ");
      if (poptions->execute_command_args != NULL)
	strcat (args[offset], poptions->execute_command_args);
      args[offset + 1] = NULL;
      if (! poptions->quiet)
	{
	  if (poptions->timeout == 0)
	    printf ("[PoCC] Running program %s\n", args[offset]);
	  else
	    printf ("[PoCC] Running program %s (with timeout of %ds)\n",
		     args[offset], poptions->timeout);
	}
      poptions->program_exec_result =
	pocc_exec_string_noexit (args, POCC_EXECV_HIDE_OUTPUT);
      if (poptions->program_exec_result == NULL)
	{
	  if (! poptions->quiet)
	    {
	      if (poptions->timeout == 0)
		printf ("[PoCC] Program %s aborted\n", args[offset]);
	      else
		printf ("[PoCC] Program %s aborted (timeout of %ds)\n",
			args[offset], poptions->timeout);
	    }
	  return EXIT_FAILURE;
	}
    }

  return EXIT_SUCCESS;
}


/*
* add tags to parallel/vector loops found-by-pluto in clast
*/
int annotate_loops( osl_scop_p program , struct clast_stmt *root){

  int j, nclastloops, nclaststmts;
  struct clast_for **clastloops = NULL;
  int *claststmts = NULL;


    osl_loop_p ll = osl_generic_lookup(program->extension, OSL_URI_LOOP);
    while(ll){
      //for each loop
  
        osl_loop_p loop = ll;
  
        //osl_loop_dump(stdout, loop);
        ClastFilter filter = {loop->iter, loop->stmt_ids, 
                                     loop->nb_stmts, subset};
        clast_filter(root, filter, &clastloops, &nclastloops, 
                                     &claststmts, &nclaststmts);
  
        /* There should be at least one */
        if (nclastloops==0) {  //FROM PLUTO
           /* Sometimes loops may disappear (1) tile size larger than trip count
           * 2) it's a scalar dimension but can't be determined from the
           * trans matrix */
           printf("Warning: parallel poly loop not found in AST\n");
           ll = ll->next;
           continue;
        }
        for (j=0; j<nclastloops; j++) {
  
          if(loop->directive & CLAST_PARALLEL_VEC){
            clastloops[j]->parallel += CLAST_PARALLEL_VEC;
          }
  
          if(loop->directive & CLAST_PARALLEL_OMP) {
            clastloops[j]->parallel += CLAST_PARALLEL_OMP;
            clastloops[j]->private_vars = strdup(loop->private_vars);
          }
        }
  

      if(clastloops){ free(clastloops); clastloops=NULL;}
      if(claststmts){ free(claststmts); claststmts=NULL;}

      ll = ll->next;
    }

  return 0;
}


/*sort scops in decending order of coordinates*/
void sort_scops(osl_scop_p *scop){
  if(*scop==NULL || (*scop)->next==NULL)
    return *scop;
  osl_coordinates_p co = NULL;
  osl_coordinates_p nco = NULL;
  int firstnode= 1;
  osl_scop_p head = *scop;

  //start from head 
  osl_scop_p listend = NULL; //last element in correct position
  while(listend != head){
    osl_scop_p node = head; //start from beginning each time
    osl_scop_p prev = head;
    //push this element as far in the list as it could go
    while(node->next != listend){
      
      co = osl_generic_lookup(node->extension, OSL_URI_COORDINATES);
      nco = osl_generic_lookup(node->next->extension, OSL_URI_COORDINATES);
      if(co->line_start < nco->line_start){
        //swap
        osl_scop_p next = node->next; 
        node->next = next->next;
        next->next = node;

        if(node==head){
          head = next;
          node = next;
        }
        else{
          node = next;
          prev->next = next;
        }
      }

      prev = node;
      node = node->next;
    }

    //update listend to the last sorted element
    listend = node;
  } 

  *scop = head;
}

static void print_macros(FILE *file)
{
    fprintf(file, "/* Useful macros. */\n") ;
    fprintf(file,
        "#define floord(n,d) (((n)<0) ? -((-(n)+(d)-1)/(d)) : (n)/(d))\n");
    fprintf(file,
        "#define ceild(n,d)  (((n)<0) ? -((-(n))/(d)) : ((n)+(d)-1)/(d))\n");
    fprintf(file, "#define max(x,y)    ((x) > (y) ? (x) : (y))\n") ; 
    fprintf(file, "#define min(x,y)    ((x) < (y) ? (x) : (y))\n\n") ; 
}

void print_preamble( char* filename, s_pocc_options_t* poptions){

  FILE* body_file = fopen (filename, "w");
  if (body_file == NULL)
    pocc_error ("Cannot open preamble file %s\n", filename);

  if (poptions->pluto_parallel)  {
      fprintf(body_file, "#include <omp.h>\n\n");
  }

  print_macros(body_file);
  
  if (poptions->pluto_multipipe) {
      fprintf(body_file, "\tomp_set_nested(1);\n");
      fprintf(body_file, "\tomp_set_num_threads(2);\n");
  }

  fclose(body_file);

}
/**
 *  Generate code for transformed scop.
 *
 * (1) Create fake tile iterators inside the .scop: polyhedral
 *     tiling performed by Pluto does not update the iterators list.
 * (2) Generate statement macros.
 * (3) Convert the .scop to CloogProgram structure.
 * (4) Generate declaration for the new iterators.
 * (5) Generate polyhedral scanning code with CLooG algorithm
 * (6) Call pocc_driver_clastops, to generate AST-based code and
 *     pretty-print it.
 * (7) Final post-processing using PoCC internal scripts (timer
 *     code, unrolling, etc.) and full code generation.
 *
 */
void
pocc_driver_codegen (osl_scop_p program,
		     s_pocc_options_t* poptions,
		     s_pocc_utils_options_t* puoptions)
{
  if (! poptions->quiet)
    printf ("[PoCC] Starting Codegen\n");
  /* Backup the default output file. */
  char *infile_name[2048];
  char *tmpfile_name[2048];
  char *outfile_name[2048];
  int scopnum = 0;

  FILE* body_file = fopen (".body.c", "w");
  if (body_file == NULL)
    pocc_error ("Cannot create file .body.c\n");
  poptions->output_file = body_file;

  /* (2) Generate polyhedral scanning code with CLooG. */
  if (! poptions->quiet)
    printf ("[PoCC] Running CLooG\n");
  CloogOptions* coptions = poptions->cloog_options;
  if (coptions == NULL)
  {
    CloogState* cstate = cloog_state_malloc ();
    poptions->cloog_options = coptions = cloog_options_malloc (cstate);

    coptions->language = 'c';
  
    if (poptions->cloog_f != POCC_CLOOG_UNDEF)
      coptions->f = poptions->cloog_f;
    if (poptions->cloog_l != POCC_CLOOG_UNDEF)
      coptions->l = poptions->cloog_l;
  }


  /* Backup the default output file. */
  FILE* out_file = poptions->output_file;
  char* preamble_file = ".preamble";

  strcpy(infile_name, poptions->input_file_name);

  sort_scops( &program); //decending order for writing in file
  osl_scop_p tmpscop = program;
  while(tmpscop){

    //To suppress cloog warning!!
    coptions->l = tmpscop->statement->scattering->nb_output_dims;

    CloogInput *input = cloog_input_from_osl_scop(coptions->state, tmpscop);
    
    //cloog_input_dump_cloog(stdout, input, cloogOptions);
    struct clast_stmt *root = cloog_clast_create_from_input(input, coptions);
    //   - mark parallel/vector loops
    annotate_loops(tmpscop, root);

    /* (3) Call Clast modules (and pretty-print if required). */
      pocc_driver_clastops_coordinates (program, root, poptions, puoptions, infile_name, scopnum);


    fclose (poptions->output_file);

    //remove the temporary file
    if(scopnum >= 1)
      remove(infile_name);

    strcpy(infile_name,tmpfile_name);
    scopnum++;

    tmpscop = tmpscop->next;
  }



  /* Perform PoCC-specific syntactic post-processing. */
  if(poptions->output_file_name == NULL){
    char *token = NULL;
    token = strtok(poptions->input_file_name, ".");    
    strcat(outfile_name, token);
    while(token){
      token = strtok(NULL, ".");    
      if(!strcmp(token, ".c") )
        strcat(outfile_name, ".pocc.c");
      else
        strcat(outfile_name, token);
    }
    
    poptions->output_file_name = strdup(outfile_name); 
  }
  pocc_driver_codegen_post_processing (body_file, poptions);

  remove (tmpfile_name);
  remove (preamble_file);

  /* (5) Build the final output file template. */
  if (! poptions->quiet)
    printf ("[PoCC] Output file is %s.\n", poptions->output_file_name);

  /* Restore the default output file. */
  poptions->output_file = out_file;
}
