#!/bin/sh
## install.sh for pocc in /Users/pouchet
##
## Made by Louis-Noel Pouchet
## Contact: <louis-noel.pouchet@inria.fr>
##
## Started on  Thu Apr 16 19:39:57 2009 Louis-Noel Pouchet
## Last update Wed Apr 29 17:45:25 2009 Louis-Noel Pouchet
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
## To Change version on the fly, after the first installation, use
## bin/pocc-util alternate <version>
##
POCC_VERSION="base";


## (1) Self bootstrap, if needed.
FORCE="";
if ! [ -f ./configure ]; then
    echo "[PoCC] Bootstrap...";
    aclocal -I driver/autoconf;
    libtoolize --force --copy;
    autoreconf -vfi;
    FORCE=y;
fi;

## (2) Extract generators/scripts/annotations.
if ! [ -d "generators/scripts/annotations" ]; then
    echo "[PoCC] Inflate archives...";
    cd generators/scripts && tar xzf annotations.tar.gz;
fi;

## (3) Configure pocc.
echo "[PoCC] Configure...";
if ! [ -f "Makefile" ] || ! [ -z "$FORCE" ]; then
    ./configure --exec-prefix=`pwd` --bindir=`pwd`/bin --libdir=`pwd`/driver/install-pocc/lib --includedir=`pwd`/driver/install-pocc/include --datarootdir=`pwd`  --disable-static --enable-shared;
fi;

## (4) Build and install pocc-utils
echo "[PoCC] Make pocc-utils...";
needed_pu=`find driver/pocc-utils -newer driver/install-pocc/include/pocc-utils 2>&1 | grep -v ".svn"`;
if ! [ -z "$needed_pu" ]; then
    cd driver/pocc-utils && make && make install && cd -;
    if [ $? -ne 0 ]; then echo "[PoCC] pocc-utils: fatal error"; exit 1; fi;
fi;

## (5) Checkout all files, and build all modules.
echo "[PoCC] Checkout and build for configuration $POCC_VERSION...";
bin/pocc-util alternate $POCC_VERSION;
if [ $? -ne 0 ]; then echo "[PoCC] fatal error"; exit 1; fi;

## (6) Build and install pocc.
echo "[PoCC] Make pocc...";
needed=`find driver/pocc -newer driver/install-pocc/include/pocc 2>&1 | grep -v ".svn"`;
if ! [ -z "$needed" ] || ! [ -z "$needed_pu" ]; ; then
    make && make install;
    if [ $? -ne 0 ]; then echo "[PoCC] fatal error"; exit 1; fi;
fi;

echo "[PoCC] Installation complete.";
