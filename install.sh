#!/bin/sh
## install.sh for pocc in /Users/pouchet
##
## Made by Louis-Noel Pouchet
## Contact: <louis-noel.pouchet@inria.fr>
##
## Started on  Thu Apr 16 19:39:57 2009 Louis-Noel Pouchet
## Last update Mon Apr 27 22:40:26 2009 Louis-Noel Pouchet
##

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
    ./configure --exec-prefix=`pwd` --bindir=`pwd`/bin --libdir=`pwd`/pocc/driver/install-pocc/lib --includedir=`pwd`/pocc/driver/install-pocc/include  --disable-static --enable-shared;
fi;

## (3) Build and install pocc-utils
echo "[PoCC] Make pocc-utils...";
cd pocc/driver/pocc-utils && make && make install && cd -;
if [ $? -ne 0 ]; then echo "[PoCC] pocc-utils: fatal error"; exit 1; fi;

## (4) Checkout all files.
echo "[PoCC] Checkout all required files...";
bin/pocc-util checkout trunk;

## (5) Build all projects.
echo "[PoCC] Make all projects...";
bin/pocc-util make;
if [ $? -ne 0 ]; then echo "[PoCC] fatal error"; exit 1; fi;

## (6) Build and install pocc.
echo "[PoCC] Make pocc...";
make && make install;
if [ $? -ne 0 ]; then echo "[PoCC] fatal error"; exit 1; fi;
echo "[PoCC] Installation complete.";
