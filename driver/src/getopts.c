/*
 * getopts.c: this file is part of the PoCC project.
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

#include <stdlib.h>
#include <unistd.h>
#include "getopts.h"


static void		print_error(char			*arg1,
				    char			*arg2,
				    const struct s_option	*option)
{
  int			s = 0;
  char			buffer[512];
  char			*mem;

  if (!option->error_print_active)
    return;
  mem = buffer;
  while (++s && *arg1)
    *(mem++) = *(arg1++);
  while (++s && *arg2)
    *(mem++) = *(arg2++);
  *(mem++) = '\n';
  *mem = '\0';
  write(1, buffer, s);
}


static int		my_strcmp(const char *s1, char *s2)
{
  for (; s1 && s2 && *s1 && *s2 && *s1 == *s2; s1++, s2++)
    ;

  return s1 && s2 && *s1 == *s2;
}


static int		treat_short_opt(char			*arg,
					int			i,
					const struct s_option	*opt)
{
  char			tmp;

  if (opt->opts[i].expect_arg)
    {
      if (!(arg[1]))
	return i;
      else
	{
	  tmp = arg[1];
	  arg[1] = '\0';
	  print_error(opt->errors[2], arg, opt);
	  arg[1] = tmp;
	}
    }
  return -1;
}


static int		get_short_opt(char			**opt_tab,
				      char			**arg,
				      const struct s_option	*option)
{
  int			i;
  int			mem = 0;
  int			found = 0;
  char			err[] = "E";

  while (**arg)
    {
      mem = -1;
      for (i = 0; i < option->nb_opts; ++i)
	if (option->opts[i].short_opt == **arg)
	  {
	    opt_tab[i] = *arg;
	    if ((found = treat_short_opt(*arg, i, option)) != -1)
	      return found;
	    found = mem = i;
	  }
      err[0] = **arg;
      if (mem == -1)
	print_error(option->errors[0], err, option);
      (*arg)++;
    }
  if (found)
    return found;
  else
    return -1;
}


static int		get_long_opt(char			**opt_tab,
				     char			*arg,
				     const struct s_option	*option)
{
  int			i;

  for (i = 0; i < option->nb_opts; ++i)
    if (my_strcmp(option->opts[i].long_opt, arg))
      {
	opt_tab[i] = arg;
	return i;
      }
  return -1;
}


static int		is_expecting_arg(int			last,
					 const struct s_option	*option)
{
  if (last == -1)
    return -1;
  if (option->opts[last].expect_arg)
      return last;
  return -1;
}


static int		treat_arg(char			**opt_tab,
				  char			*arg,
				  int			*last,
				  const struct s_option	*option)
{
  int			arg_idx = 0;

  if ((arg_idx = is_expecting_arg(*last, option)) == -1)
    return 1;
  else
    {
      opt_tab[arg_idx] = arg;
      *last = -1;
    }
  return 0;
}



/*
** NAME:	get_cmdline_opts
** SYNOPSIS:	fill the opt_tab variable with the command line arguments
** LIMITATION:	errors cant be returned inside struct s_option, and there is
**		no arg left.
** ARG1:	struct s_option previously const defined
** ARG2:	starting position
** ARG3:	main argc
** ARG4:	main argv
** ARG5:	char ** representing output options vector
** RETURN:	next arg to interpret (arg not matching option pattern)
*/
int		get_cmdline_opts(const struct s_option	*option,
				 int			start,
				 int			argc,
				 char			**argv,
				 char			**opt_tab)
{
  int		i;
  int		last = -1;

  for (i = start; i < argc; ++i)
    if (*(argv[i]) == '-')
      {
	/// ABM, 06/07/06.
	if (last != -1 && is_expecting_arg(last, option) == last)
	  {
	    if (treat_arg(opt_tab, argv[i], &last, option))
	      break;
	    else
	      continue;
	  }

	if (last != -1 && (is_expecting_arg(last, option) == last))
	  print_error(option->errors[2], argv[i - 1], option);
	(argv[i])++;
	if (*argv[i] && *argv[i] == '-')
	  {
	    (argv[i])++;
	    if ((last = get_long_opt(opt_tab, argv[i], option)) == -1)
	      print_error(option->errors[1], argv[i], option);
	  }
	else
	  last = get_short_opt(opt_tab, &(argv[i]), option);
      }
    else
      if (treat_arg(opt_tab, argv[i], &last, option))
	break;
  if (last != -1 && (is_expecting_arg(last, option) == last))
    print_error(option->errors[2], argv[i - 1], option);

  return i;
}
