dnl
dnl pocc.m4: this file is part of the PoCC project.
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


AC_DEFUN([POCC_ARG_LIBS_DEPENDENCIES],
[
dnl Add $prefix to the library path (convenience).
  if test -e ${prefix}/include; then
    CPPFLAGS="${CPPFLAGS} -I${prefix}/include"
    CFLAGS="${CFLAGS} -I${prefix}/include"
  fi;
  if test -e ${prefix}/lib; then
    LDFLAGS="${LDFLAGS} -L${prefix}/lib"
  fi;
dnl Offer --with-fm.
  AC_ARG_WITH(fm,
	      AC_HELP_STRING([--with-fm=DIR],
              	             [DIR Location of FM package]),
              [with_fm=$withval;
	       CPPFLAGS="${CPPFLAGS} -I$withval/include";
	       LDFLAGS="${LDFLAGS} -L$withval/lib"
	      ],
              [with_fm=yes])
dnl Check for fm existence.
  AS_IF([test "x$with_fm" != xno],
	[AC_CHECK_LIB([fm], [fm_solver],
	 [LIBS="-lfm $LIBS";
	  AC_DEFINE([HAVE_LIBFM], [1], [Define if you have libfm])
         ],
         [if test "x$with_fm" != xcheck; then
           AC_MSG_FAILURE([Test for FM failed. Use --with-fm to specify libfm path.])
          fi
         ])
	])
dnl Offer --with-candl.
  AC_ARG_WITH(candl,
	      AC_HELP_STRING([--with-candl=DIR],
              	             [DIR Location of CANDL package]),
              [with_candl=$withval;
	       CPPFLAGS="${CPPFLAGS} -I$withval/include";
	       LDFLAGS="${LDFLAGS} -L$withval/lib"
	      ],
              [with_candl=yes])
dnl Check for candl existence.
  AS_IF([test "x$with_candl" != xno],
	[AC_CHECK_LIB([candl], [candl_dependence],
	 [LIBS="-lcandl $LIBS";
	  AC_DEFINE([HAVE_LIBCANDL], [1], [Define if you have libcandl])
         ],
         [if test "x$with_candl" != xcheck; then
           AC_MSG_FAILURE([Test for Candl failed. Use --with-candl to specify libcandl path.])
          fi
         ])
	])
dnl Offer --with-letsee.
  AC_ARG_WITH(letsee,
	      AC_HELP_STRING([--with-letsee=DIR],
              	             [DIR Location of LetSee package]),
              [with_letsee=$withval;
	       CPPFLAGS="${CPPFLAGS} -I$withval/include";
	       LDFLAGS="${LDFLAGS} -L$withval/lib"
	      ],
              [with_letsee=yes])
dnl Check for letsee existence.
  AS_IF([test "x$with_letsee" != xno],
	[AC_CHECK_LIB([letsee], [ls_space],
	 [LIBS="-lletsee $LIBS";
	  AC_DEFINE([HAVE_LIBLETSEE], [1], [Define if you have libletsee])
         ],
         [if test "x$with_letsee" != xcheck; then
           AC_MSG_FAILURE([Test for Letsee failed. Use --with-letsee to specify libletsee path.])
          fi
         ])
	])
])
