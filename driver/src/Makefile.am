#
# Makefile.am: this file is part of the PoCC project.
#
# PoCC, the Polyhedral Compiler Collection package
#
# Copyright (C) 2009 Louis-Noel Pouchet
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation; either version 2.1
# of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# The complete GNU General Public Licence Notice can be found as the
# `COPYING.LESSER' file in the root directory.
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
	-I$(top_builddir)/driver/src					\
	-I$(top_builddir)/driver/pocc/include				\
	-I$(top_builddir)/driver/pocc-utils/include			\
	-I$(top_srcdir)							\
	-I$(top_srcdir)/driver/src					\
	-I$(top_srcdir)/driver/pocc/include				\
	-I$(top_srcdir)/driver/pocc-utils/include			\
	-I$(install_prefix)						\
	-I$(install_prefix)/optimizers/install-letsee/include		\
	-I$(install_prefix)/optimizers/install-pluto/include		\
	-I$(install_prefix)/optimizers/install-storcompacter/include	\
	-I$(install_prefix)/optimizers/install-vectorizer/include	\
	-I$(install_prefix)/ir/install-scoplib/include			\
	-I$(install_prefix)/ir/install-scoptools/include		\
	-I$(install_prefix)/analyzers/install-clan/include		\
	-I$(install_prefix)/analyzers/install-candl/include		\
	-I$(install_prefix)/analyzers/install-polyfeat/include		\
	-I$(install_prefix)/generators/install-cloog/include		\
	-I$(install_prefix)/generators/install-pragmatize/include	\
	-I$(install_prefix)/generators/install-clasttools/include	\
	-I$(install_prefix)/math/install-piplib/include			\
	-I$(install_prefix)/math/install-polylib/include		\
	-I$(install_prefix)/math/install-fm/include			\
	-I$(install_prefix)/math/external/install/include

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
