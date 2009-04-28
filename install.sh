#!/bin/sh
## install.sh for pocc in /Users/pouchet
##
## Made by Louis-Noel Pouchet
## Contact: <louis-noel.pouchet@inria.fr>
##
## Started on  Thu Apr 16 19:39:57 2009 Louis-Noel Pouchet
## Last update Tue Apr 28 18:57:46 2009 Louis-Noel Pouchet
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
POCC_VERSION="devel";


## (1) Self bootstrap, if needed.
FORCE="";
if ! [ -f ./configure ]; then
    echo "[PoCC] Bootstrap...";
    aclocal -I driver/autoconf;
    libtoolize --force --copy;
    autoreconf -vfi;
    FORCE=y;
fi;

## (2) Configure pocc.
echo "[PoCC] Configure...";
if ! [ -f "Makefile" ] || ! [ -z "$FORCE" ]; then
    ./configure --exec-prefix=`pwd` --bindir=`pwd`/bin --libdir=`pwd`/driver/install-pocc/lib --includedir=`pwd`/driver/install-pocc/include --datarootdir=`pwd`  --disable-static --enable-shared;
fi;

## (3) Build and install pocc-utils
echo "[PoCC] Make pocc-utils...";
needed=`find driver/pocc-utils -newer driver/install-pocc/include/pocc-utils | grep -v ".svn"`;
if ! [ -z "$needed" ]; then
    cd driver/pocc-utils && make && make install && cd -;
    if [ $? -ne 0 ]; then echo "[PoCC] pocc-utils: fatal error"; exit 1; fi;
fi;

## (4) Checkout all files, and build all modules.
echo "[PoCC] Select alternate configuration $POCC_VERSION...";
bin/pocc-util alternate $POCC_VERSION;
if [ $? -ne 0 ]; then echo "[PoCC] fatal error"; exit 1; fi;

## (5) Build and install pocc.
echo "[PoCC] Make pocc...";
needed=`find driver/pocc -newer driver/install-pocc/include/pocc | grep -v ".svn"`;
if ! [ -z "$needed" ]; then
    make && make install;
    if [ $? -ne 0 ]; then echo "[PoCC] fatal error"; exit 1; fi;
fi;
echo "[PoCC] Installation complete.";
