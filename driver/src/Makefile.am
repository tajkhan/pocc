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
	-I$(install_prefix)/optimizers/install-dlml/include		\
	-I$(install_prefix)/ir/install-scoplib/include			\
	-I$(install_prefix)/ir/install-scoptools/include		\
	-I$(install_prefix)/ir/install-past/include			\
	-I$(install_prefix)/ir/install-irconverter/include		\
	-I$(install_prefix)/analyzers/install-clan/include		\
	-I$(install_prefix)/analyzers/install-candl/include		\
	-I$(install_prefix)/analyzers/install-polyfeat/include		\
	-I$(install_prefix)/generators/install-cloog/include		\
	-I$(install_prefix)/generators/install-pragmatize/include	\
	-I$(install_prefix)/generators/install-clasttools/include	\
	-I$(install_prefix)/generators/install-clastannotation/include	\
	-I$(install_prefix)/generators/install-ptile/include		\
	-I$(install_prefix)/generators/install-punroller/include	\
	-I$(install_prefix)/math/install-piplib-hybrid/include		\
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


pocc_LDADD	=							     \
  $(install_prefix)/driver/install-pocc/lib/libpocc.la			     \
  $(install_prefix)/driver/install-pocc/lib/libpocc-utils.la		     \
  $(install_prefix)/ir/install-past/lib/libpast.la			     \
  $(install_prefix)/ir/install-scoplib/lib/libscoplib.la		     \
  $(install_prefix)/ir/install-scoptools/lib/libscoptools.la		     \
  $(install_prefix)/ir/install-irconverter/lib/libirconverter.la	     \
  $(install_prefix)/analyzers/install-clan/lib/libclan.la		     \
  $(install_prefix)/analyzers/install-candl/lib/libcandl.la		     \
  $(install_prefix)/analyzers/install-polyfeat/lib/libpolyfeat.la	     \
  $(install_prefix)/math/install-isl/lib/libisl.la			     \
  $(install_prefix)/math/install-piplib-hybrid/lib/libpiplib64.la	     \
  $(install_prefix)/math/install-piplib-gmp/lib/libpiplib-gmp.la	     \
  $(install_prefix)/optimizers/install-letsee/lib/libletsee.la		     \
  $(install_prefix)/optimizers/install-pluto/lib/libpluto.la		     \
  $(install_prefix)/optimizers/install-pvectorizer/lib/libpvectorizer.la     \
  $(install_prefix)/generators/install-cloog/lib/libcloog-isl.la	     \
  $(install_prefix)/generators/install-pragmatize/lib/libpragmatize.la	     \
  $(install_prefix)/generators/install-ptile/lib/libptile.la		     \
  $(install_prefix)/generators/install-punroller/lib/libpunroller.la	     \
  $(install_prefix)/generators/install-clasttools/lib/libclasttools.la	     \
  $(install_prefix)/generators/install-clastannotation/lib/libclastannotation.la  \
  $(install_prefix)/math/install-fm/lib/libfm.la

if POCC_FULL_MODE
  pocc_LDADD	+=						     \
  $(install_prefix)/optimizers/install-storcompacter/lib/libstorcompacter.la \
  $(install_prefix)/optimizers/install-vectorizer/lib/libvectorizer.la
endif

if POCC_HAS_BARVINOK
  pocc_LDADD	+=						     \
  $(install_prefix)/math/install-barvinok/lib/libbarvinok.la
endif


if POCC_HAS_HLS
  pocc_LDADD	+=							\
  $(install_prefix)/math/install-polylib/lib/libpolylibgmp.la		\
  $(install_prefix)/math/lp_solve/lpsolve55/bin/ux64/liblpsolve55.so	\
  $(install_prefix)/hls/install-lotha/lib/liblotha.la			\
  $(install_prefix)/hls/install-buffergen/lib/libbuffergen.la		\
  $(install_prefix)/hls/install-lmp/lib/liblmp.la			\
  $(install_prefix)/ir/install-hlsir/lib/libhlsir.la			\
  $(install_prefix)/math/install-barvinok/lib/libbarvinok.la
else
  pocc_LDADD	+=						     \
  $(install_prefix)/math/install-polylib/lib/libpolylib64.la
endif


#############################################################################
