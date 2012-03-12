#!/bin/sh
## checker.sh for pocc in /Users/pouchet
##
## Made by Louis-Noel Pouchet
## Contact: <louis-noel.pouchet@inria.fr>
##
## Started on  Sat Jul 18 16:57:09 2009 Louis-Noel Pouchet
## Last update Mon Mar 12 13:37:51 2012 Louis-Noel Pouchet
##


output=0
TEST_FILES="$2";
echo "[CHECK] $1";
mode="$3";
TESTS_PREFIX="$top_builddir/tests"
for i in $TEST_FILES; do
    outtemp=0;
    echo "[TEST] == $i ==";
    filename=${i%.c};
    case "$mode" in
	passthru) outfile="$filename.passthru"; options="";;
	tile) outfile="$filename.plutotile"; options="--pluto-tile";;
	tilepar) outfile="$filename.plutotilepar"; options="--pluto-tile --pluto-parallel";;
	tilemf) outfile="$filename.plutotilemf"; options="--pluto-tile --pluto-fuse maxfuse";;
	tileparmf) outfile="$filename.plutotileparmf"; options="--pluto-tile --pluto-parallel  --pluto-fuse maxfuse";;
	tileparunroll) outfile="$filename.plutotileparunroll"; options="--pluto-tile --pluto-parallel --pluto-unroll";;
	tileparunrollprev) outfile="$filename.plutotileparunrollprev"; options="--pluto-tile --pluto-parallel --pluto-unroll --pluto-prevector";;
    esac;
    ## (1) Check that generated files are the same.
    $top_builddir/driver/src/pocc $options $TESTS_PREFIX/$i -o $TESTS_PREFIX/$outfile.test.c 2>/tmp/poccout >/dev/null
    z=`diff --ignore-matching-lines='CLooG' --ignore-blank-lines $TESTS_PREFIX/$outfile.test.c $TESTS_PREFIX/$outfile.c 2>&1`
    err=`cat /tmp/poccout | grep -v "\[CLooG\] INFO:"`;
    if ! [ -z "$z" ]; then
	echo "\033[31m[FAIL] PoCC -> generated codes are different\033[0m";
	outtemp=1;
	output=1;
    elif ! [ -z "$err" ]; then
	echo "\033[31m[FAIL] PoCC -> stderr output: $err\033[0m";
	outtemp=1;
	output=1;
    fi;
    if [ "$outtemp" -eq 0 ]; then
	echo "[INFO] Generated file is conform";
    fi;
    ## (2) Check that the generated files do compile
    rm -f /tmp/poccout;
    z=`gcc $TESTS_PREFIX/$outfile.test.c -o $TESTS_PREFIX/a.out 2>&1`;
    ret="$?";
    gcchasnoomp=`echo "$z" | grep "error: omp.h: No such file"`;
    numlines=`echo "$z" | wc -l`;
    if [ "$ret" -ne 0 ] && [ "$numlines" -eq 1 ] && ! [ -z "$gcchasnoomp" ]; then
	echo "[INFO] GCC version used does not support OpenMP";
	ret=0;
    fi;
    if ! [ "$?" -eq 0 ]; then
	echo  "\033[31m[FAIL] PoCC -> generated file does not compile\033[0m";
	outtemp=1;
	output=1;
    else
	echo "[INFO] Generated file does compile correctly";
    fi;
    if [ "$outtemp" -eq 0 ]; then
	echo "[PASS] $i";
    fi;
    rm -f $TESTS_PREFIX/a.out;
done
if [ "$output" -eq 1 ]; then
    echo "\033[31m[FAIL] $1\033[0m";
else
    echo "[PASS] $1";
fi
exit $output
