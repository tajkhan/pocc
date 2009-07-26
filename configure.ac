dnl
dnl configure.ac: this file is part of the PoCC project.
dnl
dnl PoCC, the Polyhedral Compiler Collection package
dnl
dnl Copyright (C) 2009 Louis-Noel Pouchet
dnl
dnl This program is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU General Public License
dnl as published by the Free Software Foundation; either version 2
dnl of the License, or (at your option) any later version.
dnl
dnl The complete GNU General Public Licence Notice can be found as the
dnl `COPYING' file in the root directory.
dnl
dnl Author:
dnl Louis-Noel Pouchet <Louis-Noel.Pouchet@inria.fr>
dnl

#############################################################################
## Require for autoconf >= 2.60, because PolyLib needs it.
AC_PREREQ([2.60])

#############################################################################
## Fill here the @bug email adress.
AC_INIT([pocc], [1.0-beta-r2], [louis-noel.pouchet@inria.fr])
## Automatic generation of config.h
AC_CONFIG_HEADERS([driver/pocc-utils/include/pocc-utils/config.h:driver/pocc-utils/include/pocc-utils/config-h.in])
## A common file, which should be included by every source file.
AC_CONFIG_SRCDIR([driver/pocc/include/pocc/common.h])
## Put as most as possible configuration files to an auxialiry
## directory.
AC_CONFIG_AUX_DIR([driver/autoconf])
## Initialize automake. Here, a special tar version that enables
## (very) long filenames.
AM_INIT_AUTOMAKE([1.9 tar-ustar no-define foreign dist-bzip2])
#############################################################################

## Use C language.
AC_PROG_CC
## Use libtool to generate libraries.
AC_PROG_LIBTOOL


## Check for some essential headers.
AC_HEADER_STDC
AC_CHECK_HEADERS([errno.h stddef.h stdlib.h string.h strings.h unistd.h])

## Check for some essential C properties.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_C_VOLATILE

AC_CHECK_PROGS(DOXYGEN, doxygen, doxygen)
AC_CHECK_PROGS(SVN,svn)
AC_CHECK_PROGS(GIT,git)
AC_CHECK_PROGS(AUTOCONF,autoconf)
AC_CHECK_PROGS(AUTOMAKE,automake)
## Check dependencies path. This is all for devel mode.
dnl Offer --enable-devel.
AC_ARG_ENABLE(devel,
	      AC_HELP_STRING([--enable-devel=DIR],
              	             [Check for dependences for development mode]),
              [with_devel=$enableval],
              [with_devel=no])
dnl Check for fm existence.
AS_IF([test "x$with_devel" != xno],
[
if test -z "$SVN"; then AC_MSG_ERROR(svn not found); exit 1; fi;
if test -z "$GIT"; then AC_MSG_ERROR(git not found); exit 1; fi;
if test -z "$AUTOCONF"; then AC_MSG_ERROR(autoconf not found); exit 1; fi;
if test -z "$AUTOMAKE"; then AC_MSG_ERROR(automake not found); exit 1; fi;
])

#POCC_ARG_LIBS_DEPENDENCIES

#############################################################################
## Variable substitutions.
AC_SUBST([ac_aux_dir])
AC_SUBST([abs_top_srcdir])
#############################################################################
## Configure Makefiles.
AC_CONFIG_FILES([bin/pocc-util], [chmod +x bin/pocc-util])
AC_CONFIG_FILES([
  Makefile
  doc/Makefile
  driver/pocc-utils/Makefile
  driver/pocc/Makefile
  driver/src/Makefile
  tests/Makefile
  ],
  [test -z "$CONFIG_HEADERS" || echo timestamp > driver/pocc/stamp-h.in; ])
## Output the result.
AC_OUTPUT
