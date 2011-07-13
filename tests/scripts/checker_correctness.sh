#!/bin/sh
## checker_correctness.sh for PoCC in /home/pouchet/osu/projects/rice/PACE-code/trunk/polyrose
##
## Made by Louis-Noel Pouchet
## Contact: <pouchet@cse.ohio-state.edu>
##
## Started on  Tue Jul 12 14:34:28 2011 Louis-Noel Pouchet
## Last update Wed Jul 13 18:13:07 2011 Louis-Noel Pouchet
##


################################################################################
################################################################################
BASEPOCC_DIR="../driver/src";
FAILED_TESTS_DIR="failed-tests";
POLYBENCH_VERSION="polybench-2.1";
GCC="gcc -O0 -fopenmp -lm";
GCC_OTHERS="-DPOLYBENCH_DUMP_ARRAYS -I $POLYBENCH_VERSION/utilities $POLYBENCH_VERSION/utilities/instrument.c";
POCC_DEFAULT_OPTS="--quiet --use-past --pragmatizer";
################################################################################
################################################################################

check_error()
{
    if [ $1 -ne 0 ]; then
	echo "Error executing $2";
	exit $1;
    fi;
}

checkout_polybench()
{
    if ! [ -d "$POLYBENCH_VERSION" ]; then
	wget -N --quiet http://www.cse.ohio-state.edu/~pouchet/software/polybench/download/$POLYBENCH_VERSION.tar.gz;
	check_error $? "wget -N --quiet http://www.cse.ohio-state.edu/~pouchet/software/polybench/download/$POLYBENCH_VERSION.tar.gz"
	tar xzf $POLYBENCH_VERSION.tar.gz;
    fi;
}

correctness_check_file()
{
    filename="$1";
    poccopts="$2";
    poccoptsname="$3";
    RETVAL=1;
    echo "[CHECK] Testing $1 with pocc $poccopts";
    output=`$BASEPOCC_DIR/pocc $POCC_DEFAULT_OPTS $poccopts $filename -o $filename.$poccoptsname.test.c`;
    if [ $? -ne 0 ]; then
	echo "$output";
	echo "\033[31m[FAIL] $filename ($poccopts)\033[0m";
	mv $filename.$poccoptsname.test.c $FAILED_TESTS_DIR;
	echo "$filename | pocc $poccopts" >> failed.tests
	return;
    fi;
    if [ -f $filename.$poccoptsname.ref.c ]; then
	diff=`diff $filename.$poccoptsname.ref.c $filename.$poccoptsname.test.c`;
	if [ -z "$diff" ]; then
	    echo "\033[32m[PASS]\033[0m $filename ($poccopts)";
	    rm -f $FAILED_TESTS_DIR/$filename.$poccoptsname.test.c
	    RETVAL=0;
	fi;
    fi;
    if [ $RETVAL -eq 1 ]; then
	echo "[CHECK] Checking correctness of outputs...";
	if ! [ -f $filename.output ]; then
	    $GCC $GCC_OTHERS $filename -o ref;
	    ./ref 2> $filename.output;
	fi;
	$GCC $GCC_OTHERS $filename.$poccoptsname.test.c -o test;
	./test 2> $filename.test.output;
	diff=`diff $filename.test.output $filename.output`;
	if [ -z "$diff" ]; then
	    echo "\033[32m[PASS]\033[0m $filename ($poccopts)";
	    mv $filename.$poccoptsname.test.c $filename.$poccoptsname.ref.c
	    rm -f $FAILED_TESTS_DIR/$filename.$poccoptsname.test.c
	    RETVAL=0;
	else
	    echo "\033[31m[FAIL] $filename ($poccopts)\033[0m";
	    mv $filename.$poccoptsname.test.c $FAILED_TESTS_DIR;
	    echo "$filename | pocc $poccopts" >> failed.tests
	fi;
    fi;
    rm -f test ref $filename.$poccoptsname.test.c $filename.test.output $filename.output;
}

correctness_check_opts()
{
    poccoptions="$1";
    poccoptionsname="$2";
    echo "[PoCC-checker] Testing pocc $poccoptions";
    ALLRETVAL=0;
    RETVAL=0;
    for i in `find $POLYBENCH_VERSION -name "*.c" | grep -v utilities | grep -v ".ref.c" | grep -v ".test.c"`; do
	correctness_check_file "$i" "$poccoptions" "$poccoptionsname";
	if [ $RETVAL -eq 1 ]; then
	    ALLRETVAL=1;
	    ALLTESTSVAL=1;
	fi;
    done;
    if [ $ALLRETVAL -eq 0 ]; then
	echo "\033[32m[PASS] pocc $poccopts\033[0m";
    else
	echo "\033[31m[FAIL] pocc $poccopts\033[0m";
    fi;
}

################################################################################
################################################################################

echo "[PoCC-checker] Correctness checker with $POLYBENCH_VERSION";

checkout_polybench;
if ! [ -f "$BASEPOCC_DIR/pocc" ]; then
    echo "[PoCC-checker] Cannot find the pocc binary.";
    exit 1;
fi;
mkdir -p $FAILED_TESTS_DIR;
ALLTESTSVAL=0;
rm -f failed.tests;
if ! [ -z "$1" ]; then
    correctness_check_opts "$1" "user-specified";
else
    correctness_check_opts "" "passthru";
    correctness_check_opts "--pluto" "pluto";
    correctness_check_opts "--pluto --pluto-tile" "pluto-tile";
    correctness_check_opts "--pluto --pluto-tile --pluto-parallel" "pluto-tile-parallel";
fi;
if [ $ALLTESTSVAL -eq 0 ]; then
    echo "\033[32m[PASS] Correctness checker\033[0m";
else
    echo "\033[31m[FAIL] Failed test(s) summary\033[0m";
    while read n; do
	echo "\033[31m[FAIL] $n\033[0m";
    done < failed.tests;
    echo "\033[31m[FAIL] Correctness checker\033[0m";
fi;
