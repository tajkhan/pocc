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
	-I$(top_builddir)/driver/pocc/include				\
	-I$(top_builddir)/driver/pocc-utils/include			\
	-I$(top_srcdir)							\
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
	-I$(install_prefix)/analyzers/install-clan/include		\
	-I$(install_prefix)/analyzers/install-candl/include		\
	-I$(install_prefix)/analyzers/install-polyfeat/include		\
	-I$(install_prefix)/generators/install-cloog/include		\
	-I$(install_prefix)/generators/install-pragmatize/include	\
	-I$(install_prefix)/generators/install-clasttools/include	\
	-I$(install_prefix)/generators/install-clastannotation/include	\
	-I$(install_prefix)/math/install-piplib/include			\
	-I$(install_prefix)/math/install-polylib/include		\
	-I$(install_prefix)/math/install-fm/include			\
	-I$(install_prefix)/math/external/install/include

#############################################################################

pkginclude_HEADERS 	=					\
		          include/pocc/common.h			\
		          include/pocc/driver-clan.h		\
		          include/pocc/driver-candl.h		\
		          include/pocc/driver-letsee.h		\
		          include/pocc/driver-clastops.h	\
		          include/pocc/driver-codegen.h		\
		          include/pocc/driver-cloog.h		\
		          include/pocc/driver-pluto.h		\
		          include/pocc/error.h			\
		          include/pocc/exec.h			\
		          include/pocc/options.h

lib_LTLIBRARIES 	= libpocc.la


libpocc_la_SOURCES 	=				\
			  driver-clan.c			\
			  driver-candl.c		\
			  driver-letsee.c		\
			  driver-pluto.c		\
			  driver-clastops.c		\
			  driver-codegen.c		\
			  driver-cloog.c		\
			  error.c			\
			  exec.c			\
		  	  options.c			\
			  xmalloc.c

libpocc_la_CPPFLAGS	= -DHAVE_LIBPOCC_UTILS -DPOCC_ROOT_DIR=$(abs_top_srcdir)

libpocc_la_LIBADD	=						     \
  $(install_prefix)/ir/install-scoplib/lib/libscoplib.la		     \
  $(install_prefix)/ir/install-scoptools/lib/libscoptools.la		     \
  $(install_prefix)/analyzers/install-clan/lib/libclan.la		     \
  $(install_prefix)/analyzers/install-candl/lib/libcandl.la		     \
  $(install_prefix)/analyzers/install-polyfeat/lib/libpolyfeat.la	     \
  $(install_prefix)/math/install-isl/lib/libisl.la			     \
  $(install_prefix)/math/install-piplib/lib/libpiplib64.la		     \
  $(install_prefix)/math/install-polylib/lib/libpolylib64.la		     \
  $(install_prefix)/optimizers/install-letsee/lib/libletsee.la		     \
  $(install_prefix)/optimizers/install-pluto/lib/libpluto.la		     \
  $(install_prefix)/optimizers/install-storcompacter/lib/libstorcompacter.la \
  $(install_prefix)/optimizers/install-vectorizer/lib/libvectorizer.la	     \
  $(install_prefix)/generators/install-cloog/lib/libcloog-isl.la	     \
  $(install_prefix)/generators/install-pragmatize/lib/libpragmatize.la	     \
  $(install_prefix)/generators/install-clasttools/lib/libclasttools.la	     \
  $(install_prefix)/generators/install-clastannotation/lib/libclastannotation.la  \
  $(install_prefix)/math/install-fm/lib/libfm.la

## FIXME: Removed this due to a potential conflict on Solaris10. We
## already include pocc-utils in letsee, pluto, clan, candl, cloog...
#	$(install_prefix)/driver/pocc-utils/libpocc-utils.la
