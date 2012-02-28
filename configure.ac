dnl
dnl configure.ac: this file is part of the PoCC project.
dnl
dnl PoCC, the Polyhedral Compiler Collection package
dnl
dnl Copyright (C) 2009 Louis-Noel Pouchet
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public License
dnl as published by the Free Software Foundation; either version 2.1
dnl of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Lesser General Public License for more details.
dnl
dnl The complete GNU General Public Licence Notice can be found as the
dnl `COPYING.LESSER' file in the root directory.
dnl
dnl Author:
dnl Louis-Noel Pouchet <Louis-Noel.Pouchet@inria.fr>
dnl

#############################################################################
## Require for autoconf >= 2.60, because PolyLib needs it.
AC_PREREQ([2.60])

#############################################################################
## Fill here the @bug email adress.
AC_INIT([pocc], [1.0], [pouchet@cse.ohio-state.edu])
## Automatic generation of config.h
AC_CONFIG_HEADERS([driver/pocc-utils/include/pocc-utils/config.h:driver/pocc-utils/include/pocc-utils/config-h.in])
## A common file, which should be included by every source file.
AC_CONFIG_SRCDIR([driver/pocc/include/pocc/common.h])
## Put as most as possible configuration files to an auxialiry
## directory.#
AC_CONFIG_AUX_DIR([driver/autoconf])
# Initialize automake. Here, a special tar version that enables
## (very) long filenames.
AM_INIT_AUTOMAKE([1.9 tar-ustar no-define foreign dist-bzip2])
#############################################################################

## Use C language.
AC_PROG_CC
AC_PROG_CXX
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
	      AC_HELP_STRING([--enable-devel],
              	             [Check for dependences for development mode]),
              [with_devel=$enableval],
              [with_devel=no])
dnl
AS_IF([test "x$with_devel" != xno],
[
if test -z "$SVN"; then AC_MSG_ERROR(svn not found); exit 1; fi;
if test -z "$GIT"; then AC_MSG_ERROR(git not found); exit 1; fi;
if test -z "$AUTOCONF"; then AC_MSG_ERROR(autoconf not found); exit 1; fi;
if test -z "$AUTOMAKE"; then AC_MSG_ERROR(automake not found); exit 1; fi;
AC_DEFINE([HAVE_POCC_DEVEL], [1], [Define if PoCC is compiled in devel mode])
])

dnl Offer --enable-release.
AC_ARG_ENABLE(release,
	      AC_HELP_STRING([--enable-release],
              	             [Check for dependences for development mode]),
              [with_release=$enableval
	      AC_DEFINE([POCC_RELEASE_MODE], [1],
	                [Define if PoCC is compiled in release mode])
	      ],
              [with_release=no])
AM_CONDITIONAL(POCC_RELEASE_MODE, test x$with_release != xno)
AM_CONDITIONAL(POCC_FULL_MODE, test x$with_release = xno)


dnl Offer --enable-barvinok.
AC_ARG_ENABLE(barvinok,
	      AC_HELP_STRING([--enable-barvinok],
              	             [Uses barvinok (HLS mode only)]),
              [with_barvinok=$enableval
	      AC_DEFINE([POCC_HAS_BARVINOK], [1],
	                [Define if PoCC is compiled with barvinok])
	      ],
              [with_barvinok=no])
AM_CONDITIONAL(POCC_HAS_BARVINOK, test x$with_barvinok != xno)

dnl Offer --enable-hls.
AC_ARG_ENABLE(hls,
	      AC_HELP_STRING([--enable-hls],
              	             [Uses HLS mode]),
              [with_hls=$enableval
	      AC_DEFINE([POCC_HAS_HLS], [1],
	                [Define if PoCC is compiled with HLS support])
# Make sure Boost is present and useable.
AX_BOOST_BASE([1.35.0], [], [AC_MSG_ERROR([
========================================
|    ERROR                             |
========================================
| Boost 1.35.0 or later is required.   |
========================================
])])
# Make sure Rose is present and useable.
dnl All of this should go into a separate AX_ROSE macro someday.
AC_ARG_WITH([rose], [AS_HELP_STRING([--with-rose], [defines the root directory of a local Rose installation.])], [ROSE_ROOT=$withval], [])
ROSE_CPPFLAGS="-I$ROSE_ROOT/include"
ROSE_LDFLAGS="-L$ROSE_ROOT/lib -lrose"

dnl Check for rose.h
CPPFLAGS_BACKUP=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $ROSE_CPPFLAGS $BOOST_CPPFLAGS"
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([rose.h], [], [AC_MSG_ERROR([
========================================
|    ERROR                             |
========================================
| Rose 0.9.4a is required.             |
========================================
])],
[/* rose.h complains if preceded by certain standard includes */])
AC_LANG_POP([C++])
CPPFLAGS=$CPPFLAGS_BACKUP
AC_SUBST(ROSE_CPPFLAGS)
AC_SUBST(ROSE_LDFLAGS)],
              [with_hls=no])
AM_CONDITIONAL(POCC_HAS_HLS, test x$with_hls != xno)





dnl POCC_ARG_LIBS_DEPENDENCIES

dnl Send the install prefix to pocc-util
install_prefix=$prefix

#############################################################################
## Variable substitutions.
AC_SUBST([ac_aux_dir])
AC_SUBST([abs_top_srcdir])
AC_SUBST([install_prefix])
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
