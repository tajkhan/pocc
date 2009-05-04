#
# Makefile.am: this file is part of the PoCC project.
#
# PoCC, the Polyhedral Compiler Collection package
#
# Copyright (C) 2009 Louis-Noel Pouchet
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# The complete GNU General Public Licence Notice can be found as the
# `COPYING' file in the root directory.
#
# Author:
# Louis-Noel Pouchet <Louis-Noel.Pouchet@inria.fr>
#

#############################################################################
SUBDIRS 		=

#############################################################################
MAINTAINERCLEANFILES	= Makefile.in

INCLUDES		=					\
	-I$(top_builddir)					\
	-I$(top_builddir)/driver/pocc/include			\
	-I$(top_builddir)/driver/pocc-utils/include		\
	-I$(top_builddir)/optimizers/install-letsee/include	\
	-I$(top_builddir)/optimizers/install-pluto/include	\
	-I$(top_builddir)/ir/install-scoplib/include		\
	-I$(top_builddir)/analyzers/install-clan/include	\
	-I$(top_builddir)/analyzers/install-candl/include	\
	-I$(top_builddir)/analyzers/install-polyfeat/include	\
	-I$(top_builddir)/generators/install-cloog/include	\
	-I$(top_builddir)/math/install-piplib/include		\
	-I$(top_builddir)/math/install-polylib/include		\
	-I$(top_builddir)/math/install-fm/include		\
	-I$(top_srcdir)						\
	-I$(top_srcdir)/driver/pocc/include			\
	-I$(top_srcdir)/driver/pocc-utils/include		\
	-I$(top_srcdir)/optimizers/install-letsee/include	\
	-I$(top_srcdir)/optimizers/install-pluto/include	\
	-I$(top_srcdir)/ir/install-scoplib/include		\
	-I$(top_srcdir)/analyzers/install-clan/include		\
	-I$(top_srcdir)/analyzers/install-candl/include		\
	-I$(top_srcdir)/analyzers/install-polyfeat/include	\
	-I$(top_srcdir)/generators/install-cloog/include	\
	-I$(top_srcdir)/math/install-piplib/include		\
	-I$(top_srcdir)/math/install-polylib/include		\
	-I$(top_srcdir)/math/install-fm/include

#############################################################################

pkginclude_HEADERS 	=				\
		          include/pocc/common.h		\
		          include/pocc/driver-clan.h	\
		          include/pocc/driver-candl.h	\
		          include/pocc/driver-letsee.h	\
		          include/pocc/driver-cloog.h	\
		          include/pocc/driver-codegen.h	\
		          include/pocc/driver-pluto.h	\
		          include/pocc/error.h		\
		          include/pocc/options.h

lib_LTLIBRARIES 	= libpocc.la


libpocc_la_SOURCES 	=			\
			  driver-clan.c		\
			  driver-candl.c	\
			  driver-letsee.c	\
			  driver-pluto.c	\
			  driver-cloog.c	\
			  driver-codegen.c	\
			  error.c		\
		  	  options.c		\
			  xmalloc.c

libpocc_la_CPPFLAGS	= -DHAVE_LIBPOCC_UTILS -DPOCC_ROOT_DIR=$(abs_top_srcdir)

libpocc_la_LIBADD	=						\
	$(top_srcdir)/driver/pocc-utils/libpocc-utils.la		\
	$(top_srcdir)/ir/install-scoplib/lib/libscoplib.la		\
	$(top_srcdir)/analyzers/install-clan/lib/libclan.la		\
	$(top_srcdir)/analyzers/install-candl/lib/libcandl.la		\
	$(top_srcdir)/analyzers/install-polyfeat/lib/libpolyfeat.la	\
	$(top_srcdir)/math/install-piplib/lib/libpiplib64.la		\
	$(top_srcdir)/math/install-polylib/lib/libpolylib64.la		\
	$(top_srcdir)/optimizers/install-letsee/lib/libletsee.la	\
	$(top_srcdir)/optimizers/install-pluto/lib/libpluto.la		\
	$(top_srcdir)/generators/install-cloog/lib/libcloog-polylib.la	\
	$(top_srcdir)/math/install-fm/lib/libfm.la
