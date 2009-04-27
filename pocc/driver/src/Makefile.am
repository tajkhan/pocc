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

INCLUDES		=						\
	-I$(top_builddir)						\
	-I$(top_builddir)/pocc/driver/pocc/include			\
	-I$(top_builddir)/pocc/driver/pocc-utils/include		\
	-I$(top_builddir)/pocc/src					\
	-I$(top_builddir)/optimizers/letsee/install-letsee/include	\
	-I$(top_builddir)/optimizers/pluto/install-pluto/include	\
	-I$(top_builddir)/analyzers/clan/install-clan/include		\
	-I$(top_builddir)/analyzers/candl/install-candl/include		\
	-I$(top_builddir)/generators/cloog/install-cloog/include	\
	-I$(top_builddir)/math/piplib/install-piplib/include		\
	-I$(top_builddir)/math/polylib/install-polylib/include		\
	-I$(top_builddir)/math/fm/install-fm/include			\
	-I$(top_srcdir)							\
	-I$(top_srcdir)/pocc/driver/pocc/include			\
	-I$(top_srcdir)/pocc/driver/pocc-utils/include			\
	-I$(top_srcdir)/pocc/src					\
	-I$(top_srcdir)/optimizers/letsee/install-letsee/include	\
	-I$(top_srcdir)/optimizers/pluto/install-pluto/include		\
	-I$(top_srcdir)/analyzers/clan/install-clan/include		\
	-I$(top_srcdir)/analyzers/candl/install-candl/include		\
	-I$(top_srcdir)/generators/cloog/install-cloog/include		\
	-I$(top_srcdir)/math/piplib/install-piplib/include		\
	-I$(top_srcdir)/math/polylib/install-polylib/include		\
	-I$(top_srcdir)/math/fm/install-fm/include	

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

pocc_LDADD		= $(top_srcdir)/pocc/driver/pocc/libpocc.la

#############################################################################
