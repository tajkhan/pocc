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

INCLUDES		= -I$(top_builddir)				\
			  -I$(top_builddir)/driver/pocc-utils/include	\
			  -I$(top_srcdir)				\
			  -I$(top_srcdir)/driver/pocc-utils/include

#############################################################################

poccheaddir = $(pkgincludedir)/../pocc-utils

pocchead_HEADERS =					\
			  include/pocc-utils/options.h	\
			  include/pocc-utils/types.h

lib_LTLIBRARIES 	= libpocc-utils.la


libpocc_utils_la_SOURCES 	=	options.c
