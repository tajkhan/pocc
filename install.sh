#!/bin/sh
## install.sh for pocc in /Users/pouchet
## 
## Made by Louis-Noel Pouchet
## Contact: <louis-noel.pouchet@inria.fr>
## 
## Started on  Thu Apr 16 19:39:57 2009 Louis-Noel Pouchet
## Last update Thu Apr 16 19:52:52 2009 Louis-Noel Pouchet
##

if ! [ -f ./configure ]; then # || true
    aclocal -I pocc/driver/config
    libtoolize --force --copy
    autoreconf -vfi
fi;

./configure --prefix=`pwd`/bin --disable-static --enable-shared
bin/pocc-util checkout trunk
bin/pocc-util make
make
make install
