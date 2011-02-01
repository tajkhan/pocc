#!/bin/sh

pwd=`pwd`;
poccvars=`find . -name "*.so" | grep -v "\.libs" | xargs echo | sed -e "s~\./~$pwd/~g" | sed "s~lib/lib[-a-z0-9]*.so[ ]*~lib ~g" | sed -e "s/ /:/g"`;
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$poccvars";

