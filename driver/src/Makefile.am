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
	-I$(top_builddir)/driver/src				\
	-I$(top_builddir)/optimizers/install-letsee/include	\
	-I$(top_builddir)/optimizers/install-pluto/include	\
	-I$(top_builddir)/optimizers/install-polyfeat/include	\
	-I$(top_builddir)/ir/install-scoplib/include		\
	-I$(top_builddir)/analyzers/install-clan/include	\
	-I$(top_builddir)/analyzers/install-candl/include	\
	-I$(top_builddir)/generators/install-cloog/include	\
	-I$(top_builddir)/math/install-piplib/include		\
	-I$(top_builddir)/math/install-polylib/include		\
	-I$(top_builddir)/math/install-fm/include		\
	-I$(top_srcdir)						\
	-I$(top_srcdir)/driver/pocc/include			\
	-I$(top_srcdir)/driver/pocc-utils/include		\
	-I$(top_srcdir)/driver/src				\
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

pocc_CFLAGS		= -DPOCC_ROOT_DIR=$(abs_top_srcdir)

pocc_LDADD		= $(top_builddir)/driver/pocc/libpocc.la

#############################################################################
