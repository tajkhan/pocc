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

INCLUDES		= -I$(top_builddir)/src -I$(top_srcdir)/src	\
			  -I$(top_builddir) -I$(top_builddir)/letsee	\
			  -I$(top_srcdir) -I$(top_srcdir)/letsee	\
			  -I$(top_builddir)/letsee/include		\
			  -I$(top_srcdir)/letsee/include

#############################################################################

EXTRA_DIST		 = 

#############################################################################

noinst_HEADERS		=			\
	getopts.h				\
	options.h

bin_PROGRAMS		= pocc


pocc_SOURCES		=			\
	pocc.c					\
	getopts.c				\
	options.c

pocc_LDADD		= 						\
	$(top_srcdir)/pocc/analyzers/clan/clan-install/libclan.la	\
	$(top_srcdir)/pocc/analyzers/candl/candl-install/libcandl.la	\
	$(top_srcdir)/pocc/math/piplib/piplib-install/libpiplib.la	\
	$(top_srcdir)/pocc/math/fm/fm-install/libfm.la

#############################################################################
