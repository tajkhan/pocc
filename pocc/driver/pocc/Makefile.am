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

pkginclude_HEADERS 	=

lib_LTLIBRARIES 	= libpocc.la


libpocc_la_SOURCES 	=

AM_LDFLAGS = -lm -lfm -lcandl -lcloog -lclan
