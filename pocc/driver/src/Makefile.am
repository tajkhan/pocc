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

INCLUDES		= -I$(top_builddir) -I$(top_builddir)/pocc/driver \
			  -I$(top_srcdir) -I$(top_srcdir)/pocc/driver

#############################################################################

EXTRA_DIST		 =

#############################################################################

noinst_HEADERS		=			\
	error.h					\
	common.h				\
	getopts.h				\
	options.h

bin_PROGRAMS		= pocc


pocc_SOURCES		=			\
	xmalloc.c				\
	xstrdup.c				\
	xstrerror.c				\
	error.c					\
	pocc.c					\
	getopts.c				\
	options.c

pocc_LDADD		= 						  \
	$(top_srcdir)/analyzers/clan/install-clan/lib/libclan.la	  \
	$(top_srcdir)/analyzers/candl/install-candl/lib/libcandl.la 	  \
	$(top_srcdir)/math/piplib/install-piplib/lib/libpiplib64.la	  \
	$(top_srcdir)/math/fm/install-fm/lib/libfm.la

#############################################################################
