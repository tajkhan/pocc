#!/bin/sh
## checker.sh for pocc in /Users/pouchet
##
## Made by Louis-Noel Pouchet
## Contact: <louis-noel.pouchet@inria.fr>
##
## Started on  Sat Jul 18 16:57:09 2009 Louis-Noel Pouchet
## Last update Sat Jul 18 18:25:18 2009 Louis-Noel Pouchet
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
    esac;
    $top_builddir/driver/src/pocc $options $TESTS_PREFIX/$i -o $TESTS_PREFIX/$outfile.test.c 2>/tmp/poccout >/dev/null
    z=`diff --ignore-matching-lines='CLooG' $TESTS_PREFIX/$outfile.test.c $TESTS_PREFIX/$outfile.c 2>&1`
    err=`cat /tmp/poccout | grep -v "\[CLooG\] INFO:"`;
    if ! [ -z "$z" ]; then
	echo -e "\033[31m[FAIL] PoCC -> generated codes are different\033[0m";
	outtemp=1;
	output=1;
    elif ! [ -z "$err" ]; then
	echo -e "\033[31m[FAIL] PoCC -> stderr output: $err\033[0m";
	outtemp=1;
	output=1;
    fi
    rm -f /tmp/poccout;
    if [ "$outtemp" -eq 0 ]; then
	echo "[PASS] $i";
    fi;
done
if [ $output = "1" ]; then
    echo -e "\033[31m[FAIL] $1\033[0m";
else
    echo "[PASS] $1";
fi
exit $output
