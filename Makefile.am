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
SUBDIRS 		=			\
			  doc			\
			  driver/pocc-utils	\
			  driver/pocc		\
			  driver/src		\
			  tests


#############################################################################
ACLOCAL_AMFLAGS		= -I driver/autoconf

CONFIGURE_DEPENDENCIES 	= $(srcdir)/driver/autoconf/pocc.m4

m4datadir		= $(datadir)/aclocal

#dist_m4data_DATA	= pocc/driver/config/letsee.m4

AUX_DIST                =			\
	$(ac_aux_dir)/config.guess		\
	$(ac_aux_dir)/config.sub		\
	$(ac_aux_dir)/install-sh		\
	$(ac_aux_dir)/ltmain.sh			\
	$(ac_aux_dir)/missing			\
	$(ac_aux_dir)/depcomp


EXTRA_DIST		=			\
	install.sh				\
	README					\
	gemver.c				\
	configure.ac				\
	bin/pocc-util				\
	bin/pocc-util.in			\
	bin/pocc-alternate			\
	bin/pocc-checkout			\
	bin/pocc-make				\
	config/base/configure.cfg		\
	config/base/driver.cfg			\
	config/devel/configure.cfg		\
	config/devel/driver.cfg			\
	config/irregular/configure.cfg		\
	config/irregular/driver.cfg		\
	config/stable/configure.cfg		\
	config/stable/driver.cfg		\
	generators/scripts/annotations.tar.gz	\
	generators/scripts/compile		\
	generators/scripts/inscop		\
	generators/scripts/omp			\
	generators/scripts/plann		\
	generators/scripts/ploog		\
	generators/scripts/timercode		\
	generators/scripts/vloog		\
	math/external/gmp-4.3.1.tar.gz


MAINTAINERCLEANFILES 	=					\
	Makefile.in						\
	aclocal.m4						\
	configure						\
	driver/pocc-utils/include/pocc-utils/config-h.in	\
	driver/pocc/stamp-h.in					\
	$(AUX_DIST)

dist-hook:
	(cd $(distdir) && mkdir -p $(ac_aux_dir))
	for file in $(AUX_DIST); do \
	  cp $$file $(distdir)/$$file; \
	done
#############################################################################
