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
MAINTAINERCLEANFILES    = Makefile.in

TESTS_ENVIRONMENT       = top_builddir=$(top_builddir)

EXTRA_DIST              = scripts/checker.sh
#############################################################################
check_SCRIPTS		=			\
	scripts/checker_passthru.sh		\
	scripts/checker_plutotile.sh		\
	scripts/checker_plutotilepar.sh		\
	scripts/checker_plutotileparunroll.sh	\
	scripts/checker_ptpup.sh

BENCH_FILES		= 			\
	benchmarks/dgemm/dgemm.c		\
	benchmarks/gemver/gemver.c

TEST_SUITE		=			\
	tests/dgemm/dgemm.c			\
	tests/gemver/gemver.c

TEST_SUITE_REF_PASSTHRU    	= $(TEST_SUITE:.c=.passthru.c)
TEST_SUITE_REF_PLUTOTILE    	= $(TEST_SUITE:.c=.plutotile.c)
TEST_SUITE_REF_PLUTOTILEPAR    	= $(TEST_SUITE:.c=.plutotilepar.c)
TEST_SUITE_REF_PLUTOTILE_MF    	= $(TEST_SUITE:.c=.plutotilemf.c)
TEST_SUITE_REF_PLUTOTILEPAR_MF	= $(TEST_SUITE:.c=.plutotileparmf.c)
TEST_SUITE_REF_PLUTOTILEPARUNR 	= $(TEST_SUITE:.c=.plutotileparunroll.c)
TEST_SUITE_REF_PTPUP	 	= $(TEST_SUITE:.c=.plutotileparunrollprev.c)

TEST_SUITE_CLEAN_PASSTHRU    	= $(TEST_SUITE:.c=.passthru.test.c)
TEST_SUITE_CLEAN_PLUTOTILE    	= $(TEST_SUITE:.c=.plutotile.test.c)
TEST_SUITE_CLEAN_PLUTOTILEPAR   = $(TEST_SUITE:.c=.plutotilepar.test.c)
TEST_SUITE_CLEAN_PLUTOTILE_MF   = $(TEST_SUITE:.c=.plutotilemf.test.c)
TEST_SUITE_CLEAN_PLUTOTILEPAR_MF= $(TEST_SUITE:.c=.plutotileparmf.test.c)
TEST_SUITE_CLEAN_PLUTOTILEPARUNR= $(TEST_SUITE:.c=.plutotileparunroll.test.c)
TEST_SUITE_CLEAN_PTPUP		= $(TEST_SUITE:.c=.plutotileparunrollprev.test.c)

TEST_SUITE_CLEAN		=		\
	$(TEST_SUITE_CLEAN_PASSTHRU)		\
	$(TEST_SUITE_CLEAN_PLUTOTILE)		\
	$(TEST_SUITE_CLEAN_PLUTOTILEPAR)	\
	$(TEST_SUITE_CLEAN_PLUTOTILE_MF)	\
	$(TEST_SUITE_CLEAN_PLUTOTILEPAR_MF)	\
	$(TEST_SUITE_CLEAN_PLUTOTILEPARUNR)	\
	$(TEST_SUITE_CLEAN_PTPUP)


## Replace by program names when needed
# check_PROGRAMS = ...
TESTS				= $(check_SCRIPTS)

CLEANFILES			= $(TEST_SUITE_CLEAN)

EXTRA_DIST	+=				\
	scripts/checker_perfcorrect.sh		\
	scripts/conflist-small.txt		\
	scripts/conflist-large.txt		\
	scripts/conflist-passthru.txt		\
	scripts/checker_correctness.sh		\
	$(BENCH_FILES)				\
	$(TEST_SUITE)				\
	$(TEST_SUITE_REF_PASSTHRU)		\
	$(TEST_SUITE_REF_PLUTOTILE)		\
	$(TEST_SUITE_REF_PLUTOTILEPAR)		\
	$(TEST_SUITE_REF_PLUTOTILE_MF)		\
	$(TEST_SUITE_REF_PLUTOTILEPAR_MF)	\
	$(TEST_SUITE_REF_PLUTOTILEPARUNR)	\
	$(TEST_SUITE_REF_PTPUP)			\
	$(check_SCRIPTS)

TESTS_ENVIRONMENT +=							\
	CHECKER="$(top_srcdir)/tests/scripts/checker.sh"		\
	TEST_SUITE="$(TEST_SUITE)"

correctness-check-passthru:
	(scripts/checker_perfcorrect.sh "correctness" "scripts/conflist-passthru.txt")
correctness-check-fast:
	(scripts/checker_perfcorrect.sh "correctness" "scripts/conflist-small.txt")
correctness-check:
	(scripts/checker_perfcorrect.sh "correctness" "scripts/conflist-large.txt")
performance-check-fast:
	(scripts/checker_perfcorrect.sh "performance-small" "scripts/conflist-small.txt")
performance-check:
	(scripts/checker_perfcorrect.sh "performance" "scripts/conflist-large.txt")
performance-correctness-check-fast:
	(scripts/checker_perfcorrect.sh "correctness+performance-small" "scripts/conflist-small.txt")
performance-correctness-check:
	(scripts/checker_perfcorrect.sh "correctness+performance" "scripts/conflist-large.txt")
#############################################################################
