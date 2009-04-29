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

EXTRA_DIST		=			\
			pocc.pdf		\
			htmldoc.tar.gz

info_TEXINFOS		= pocc.texi

# The following requires a fixed version of the Emacs 19.30 etags.
ETAGS_ARGS		= --lang=none \
 --regex='/^@node[ \t]+\([^,]+\)/\1/' $(srcdir)/pocc.texi
TAGS_DEPENDENCIES	= pocc.texi doxygen.cfg


DOCDIR			= htmldoc
DOXYFILE		= doxygen.cfg
DOXYGEN			= @DOXYGEN@

htmldoc.tar.gz:
	rm -rf $(top_srcdir)/doc/htmldoc
	mkdir -p $(top_srcdir)/doc/htmldoc
	$(DOXYGEN) $(DOXYFILE)
	tar czf htmldoc.tar.gz htmldoc

#############################################################################
