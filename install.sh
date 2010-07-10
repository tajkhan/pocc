#!/bin/sh
## install.sh for pocc in /Users/pouchet
##
## Made by Louis-Noel Pouchet
## Contact: <louis-noel.pouchet@inria.fr>
##
## Started on  Thu Apr 16 19:39:57 2009 Louis-Noel Pouchet
## Last update Mon May  3 15:35:56 2010 Louis-Noel Pouchet
##

##
##
## Set the following variable to the PoCC mode to use for the
## installation. Choice is:
## - devel: all modules (development version), requires SVN access to ALCHEMY
## - base: all modules (development version)
## - irregular: all modules (development version) including those using
##               irregular programs support. Requires SVN access to ALCHEMY.
## - stable: all modules (released version)
##
## To Change the mode on the fly, after the first installation, use
## bin/pocc-util alternate <mode>
##
POCC_MODE="base";

## **** GMP section ****
##
## For the moment, we embed a working version of GMP. This should not
## be necessary, and we should require the user to install it
## himself. Only ISL requires it.
GMPVERSION="gmp-4.3.1";
## In case of problem with ISL compilation, set ABI=32 for a 32bit
## operating system on 64bits machines (for ex, Mac OS 10.4 AND 10.5), or
## ABI=64 for a 64bits operating system. Just set to "" by default.
## GMP_ABI_FORCE="ABI=32";
GMP_ABI_FORCE="";
##
## *********************

## (1) Self bootstrap, if needed.
FORCE="";
if ! [ -f ./configure ]; then
    echo "[PoCC] Bootstrap...";
    aclocal -I driver/autoconf &&
    libtoolize --force --copy &&
    autoreconf -vfi;
    if [ $? -ne 0 ]; then echo "[PoCC] bootstrap: fatal error"; exit 1; fi;
    FORCE=y;
fi;

## (2) Extract generators/scripts/annotations.
echo "[PoCC] Inflate archives...";
if ! [ -d "generators/scripts/annotations" ]; then
    cd generators/scripts && tar xzf annotations.tar.gz; cd -;
fi;
## (3) Extract and build GMP.
if [ -z "$POCC_INSTALL_PREFIX" ]; then
    POCC_INSTALL_PREFIX=`pwd`;
fi;
if ! [ -d "math/external/$GMPVERSION" ]; then
    cd math/external && tar xzf $GMPVERSION.tar.gz; cd -;
fi;
if ! [ -f "math/external/$GMPVERSION/Makefile" ]; then
    echo "[PoCC] Configure $GMPVERSION...";
    pt_inst="$POCC_INSTALL_PREFIX/math/external/install";
    cd math/external/$GMPVERSION && eval $GMP_ABI_FORCE ./configure --prefix=$pt_inst; if [ $? -ne 0 ]; then echo "[PoCC] configure $GMPVERSION: fatal error"; exit 1; fi;cd -;
fi;
if ! [ -f "math/external/install/include/gmp.h" ]; then
	echo "[PoCC] Build $GMPVERSION...";
	cd math/external/$GMPVERSION && make install;if [ $? -ne 0 ]; then echo "[PoCC] build $GMPVERSION: fatal error"; exit 1; fi; cd -;
fi;

## (4) Configure pocc.
echo "[PoCC] Configure...";
if ! [ -f "Makefile" ] || ! [ -z "$FORCE" ]; then
    enable_devel="--enable-devel";
    if [ "$POCC_MODE" = "stable" ] || [ "$POCC_MODE" = "local" ]; then
	enable_devel="";
    fi;
    ## Warning: must include gmp.h path for the moment into pocc driver.
    GMPINCFLAGS="CPPFLAGS='-I `pwd`/math/external/install/include'" 
    POCC_INSTALL_PREFIX=`pwd`;
    eval $GMPINCFLAGS ./configure --prefix="$POCC_INSTALL_PREFIX"  --bindir="$POCC_INSTALL_PREFIX/bin" --libdir="$POCC_INSTALL_PREFIX/driver/install-pocc/lib" --includedir="$POCC_INSTALL_PREFIX/driver/install-pocc/include" --datarootdir=`pwd`  --disable-static --enable-shared $enable_devel;
    if [ $? -ne 0 ]; then echo "[PoCC] configure: fatal error"; exit 1; fi;
fi;

## (5) Build and install pocc-utils
echo "[PoCC] Make pocc-utils...";
needed_pu=`find driver/pocc-utils -newer $POCC_INSTALL_PREFIX/driver/install-pocc/include/pocc-utils 2>&1 | grep -v ".svn"`;
if ! [ -z "$needed_pu" ]; then
    cd driver/pocc-utils && make && make install && cd -;
    if [ $? -ne 0 ]; then echo "[PoCC] pocc-utils: fatal error"; exit 1; fi;
#fi;

## (6) Checkout all files, and build all modules.
echo "[PoCC] Checkout and build for configuration $POCC_MODE...";
bin/pocc-util alternate $POCC_MODE;
if [ $? -ne 0 ]; then echo "[PoCC] fatal error"; exit 1; fi;

## (7) Build and install pocc.
echo "[PoCC] Make pocc...";
needed=`find driver/pocc -newer $POCC_INSTALL_PREFIX/driver/install-pocc/include/pocc 2>&1 | grep -v ".svn"`;
if ! [ -z "$needed" ] || ! [ -z "$needed_pu" ]; then
    make && make install;
    if [ $? -ne 0 ]; then echo "[PoCC] fatal error"; exit 1; fi;
fi;

echo "[PoCC] Installation complete.";
