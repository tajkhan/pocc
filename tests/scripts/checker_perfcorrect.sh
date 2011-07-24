#!/bin/sh
## checker_correctness.sh for PoCC in /home/pouchet/osu/projects/rice/PACE-code/trunk/polyrose
##
## Made by Louis-Noel Pouchet
## Contact: <pouchet@cse.ohio-state.edu>
##
## Started on  Tue Jul 12 14:34:28 2011 Louis-Noel Pouchet
## Last update Sun Jul 24 01:10:08 2011 Louis-Noel Pouchet
##

################################################################################
################################################################################
## Global variables (user-defined).
## Email address of the receiver of the script result.
EMAIL_MAINTAINER="pouchet@cse.ohio-state.edu";
## GCC command line for the performance check.
GCC_COMPILER_COMMAND="gcc-4.5 -O3 -fopenmp";
## String to identify GCC version used for performance. No space.
GCC_STRING_NAME="gcc-4.5";
## ICC command line for the performance check.
ICC_COMPILER_COMMAND="/opt/intel/Compiler/11.1/072/bin/intel64/icc -fast -parallel -openmp";
## String to identify ICC version used for performance. No space.
ICC_STRING_NAME="icc-11.1";
## Script to run before using ICC, to set up environment variable, if any.
ICC_COMPILER_ENVSCRIPT="/opt/intel/Compiler/11.1/072/bin/iccvars.sh intel64";
## Testsuite name of the tarball (.tar.gz) to be used.
TESTSUITE_NAME="polybench-2.1";
## Command to retrieve the test suite tarball.
TESTSUITE_GET_COMMAND="wget --quiet -N http://www.cse.ohio-state.edu/~pouchet/software/polybench/download/$TESTSUITE_NAME.tar.gz";
## Last-level cache size (in kB), used to flush the cache before performance
## measurement.
LLC_CACHE_SIZE="25000";
## Program command to generate the transformed programs.
TRANSFORMER_COMMAND="../driver/src/pocc";
## Default option(s) applied to all programs.
TRANSFORMER_DEFAULT_OPTS="--quiet";
## Regression/improvement thresold. Here, 10%.
REGRESSION_THRESOLD="0.1";


## Parallel environment setting. Here, 16 h/w threads.
export OMP_SCHEDULE=static
export OMP_DYNAMIC=FALSE
export GOMP_CPU_AFFINITY="0-15";
export KMP_SCHEDULE=static,balanced
export KMP_AFFINITY="proclist=[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]"
export OMP_NUM_THREADS=16;
export KMP_NUM_THREADS=16;


################################################################################
################################################################################
## Global variables (script-specific).
CONFLIST_SMALL="scripts/conflist-small.txt";
CONFLIST_LARGE="scripts/conflist-large.txt";
FAILED_TESTS_DIR="failed-tests";
FAILED_TEST_FILE="failed.tests";
PERF_TESTS_DIR="perf-tests";
PERF_FILE_TEMPLATE="$PERF_TESTS_DIR/perf.out"
CSV_PERF_FILE_TEMPLATE="$PERF_TESTS_DIR/perf"
GCC_CORRECT="gcc -O0 -fopenmp -lm -DPOLYBENCH_DUMP_ARRAYS";
POLYBENCH_PERF_FLAGS="-lm -DPOLYBENCH_TIME -DPOLYBENCH_CACHE_SIZE_KB=$LLC_CACHE_SIZE";
GCC_PERF="$GCC_COMPILER_COMMAND $POLYBENCH_PERF_FLAGS";
GCC_COMP_VER="$GCC_STRING_NAME";
ICC_PERF="$ICC_COMPILER_COMMAND $POLYBENCH_PERF_FLAGS";
ICC_COMP_VER="$ICC_STRING_NAME";
COMP_OTHERS=" -I $TESTSUITE_NAME/utilities $TESTSUITE_NAME/utilities/instrument.c";
POCC_DEFAULT_OPTS="--quiet";
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
    if ! [ -d "$TESTSUITE_NAME" ]; then
	$TESTSUITE_GET_COMMAND;
	check_error $? "$TESTSUITE_GET_COMMAND"
	tar xzf $TESTSUITE_NAME.tar.gz;
    fi;
}

compute_mean_exec_time()
{
    file="$1";
    cat "$file" | grep "[0-9]\+" | sort -n | head -n 4 | tail -n 3 > avg.out;
    expr="(0";
    while read n; do
	expr="$expr+$n";
    done < avg.out;
    rm -f avg.out;
    time=`echo "scale=8;$expr)/3" | bc`;
    tmp=`echo "$time" | cut -d . -f 1`;
    if [ -z "$tmp" ]; then
	time="0$time";
    fi;
    PROCESSED_TIME="$time";
}


correctness_check_file()
{
    filename="$1";
    poccopts="$2";
    poccoptsname="$3";
    comp_perf="$4";
    comp_ver="$5";
    mode="$6";
    correctness_only=`echo "$mode" | grep correctness | grep -v performance`;
    performance_only=`echo "$mode" | grep performance | grep -v correctness`;
    RETVAL=1;
    echo "[CHECK] Testing $1 with $TRANSFORMER_COMMAND $poccopts";
    output=`$TRANSFORMER_COMMAND $TRANSFORMER_DEFAULT_OPTS $poccopts $filename -o $filename.$poccoptsname.test.c`;
    if [ $? -ne 0 ]; then
	echo "$output";
	echo "\033[31m[FAIL][Correctness] $filename ($poccopts)\033[0m";
	mv $filename.$poccoptsname.test.c $FAILED_TESTS_DIR;
	echo "$filename | pocc $POCC_DEFAULT_OPTS $poccopts" >> $FAILED_TEST_FILE;
	return;
    fi;
    if [ -z "$performance_only" ]; then
	if [ -f $filename.$poccoptsname.ref.c ]; then
	    diff=`diff $filename.$poccoptsname.ref.c $filename.$poccoptsname.test.c`;
	    if [ -z "$diff" ]; then
		echo "\033[32m[PASS][Correctness]\033[0m $filename ($poccopts)";
		rm -f $FAILED_TESTS_DIR/$filename.$poccoptsname.test.c
		RETVAL=0;
	    fi;
	fi;
	if [ $RETVAL -eq 1 ]; then
	    echo "[CHECK] Checking correctness of outputs...";
	    if ! [ -f $filename.output ]; then
		out=`$GCC_CORRECT $COMP_OTHERS $filename -o ref`;
		out=`./ref 2> $filename.output`;
	    fi;
	    out=`$GCC_CORRECT $COMP_OTHERS $filename.$poccoptsname.test.c -o test`;
	    out=`./test 2> $filename.test.output`;
	    diff=`diff $filename.test.output $filename.output`;
	    if [ -z "$diff" ]; then
		echo "\033[32m[PASS][Correctness]\033[0m $filename ($poccopts)";
		mv $filename.$poccoptsname.test.c $filename.$poccoptsname.ref.c
		rm -f $FAILED_TESTS_DIR/$filename.$poccoptsname.test.c
		RETVAL=0;
	    else
		echo "\033[31m[FAIL][Correctness] $filename ($poccopts)\033[0m";
		mv $filename.$poccoptsname.test.c $FAILED_TESTS_DIR;
		echo "$filename | pocc $POCC_DEFAULT_OPTS $poccopts" >> $FAILED_TEST_FILE;
	    fi;
	fi;
    fi;
    if [ -z "$correctness_only" ]; then
	if [ -f $filename.$poccoptsname.ref.c ] &&
	    [ -f $filename.$poccoptsname.ref.c.time.$comp_ver ]; then
	    diff=`diff $filename.$poccoptsname.ref.c $filename.$poccoptsname.test.c`;
	    if [ -z "$diff" ]; then
		echo "\033[32m[No change]\033[0m $filename ($poccopts)";
		rm -f $FAILED_TESTS_DIR/$filename.$poccoptsname.test.c
		time=`cat $filename.$poccoptsname.ref.c.time.$comp_ver`;
		echo "$filename.$poccoptsname | $time" >> $PERF_FILE.$comp_ver;
		RETVAL=0;
	    fi;
	else
	    echo "[CHECK] Checking performance of outputs...";

	    if [ -f $filename.$poccoptsname.ref.c ]; then
		cp $filename.$poccoptsname.ref.c $filename.$poccoptsname.test.c;
	    fi;
	    rm -f test;
	    out=`$comp_perf $COMP_OTHERS $filename.$poccoptsname.test.c -o test  >/dev/null 2>/dev/null`;
	    if [ $? -ne 0 ]; then
		echo "\033[31m[FAIL][Performance] $filename ($poccopts) with $comp_ver\033[0m";
		return;
	    fi;
	    # 5 runs.
	    ./test > $filename.$poccoptsname.ref.c.time.$comp_ver 2>/dev/null;
	    ./test >> $filename.$poccoptsname.ref.c.time.$comp_ver 2>/dev/null;
	    ./test >> $filename.$poccoptsname.ref.c.time.$comp_ver 2>/dev/null;
	    ./test >> $filename.$poccoptsname.ref.c.time.$comp_ver 2>/dev/null;
	    ./test >> $filename.$poccoptsname.ref.c.time.$comp_ver 2>/dev/null;
	    ret=$?;
	    if [ $ret -ne 0 ]; then
		echo "OUTPUT with $comp_ver: $ret";
		echo "\033[31m[FAIL][Performance] $filename ($poccopts)\033[0m";
		mv $filename.$poccoptsname.test.c $FAILED_TESTS_DIR;
		echo "$filename | pocc $POCC_DEFAULT_OPTS $poccopts" >> FAILED_TEST_FILE;
		return;
	    fi;
	    compute_mean_exec_time "$filename.$poccoptsname.ref.c.time.$comp_ver";
	    echo "$filename.$poccoptsname | $PROCESSED_TIME" >> $PERF_FILE.$comp_ver;
	    echo "$PROCESSED_TIME" > $filename.$poccoptsname.ref.c.time.$comp_ver
	    echo "\033[32m[PASS][Performance]\033[0m Time: $PROCESSED_TIME | $filename ($poccopts)";
	    RETVAL=0;
	fi;
    fi;
    rm -f test ref $filename.$poccoptsname.test.c $filename.test.output $filename.output;
}

correctness_performance_check_opts()
{
    poccoptions="$1";
    poccoptionsname="$2";
    mode="$3";
    echo "[PoCC-checker] Testing pocc $poccoptions";
    ALLRETVAL=0;
    RETVAL=0;
    for i in `find $TESTSUITE_NAME -name "*.c" | grep -v utilities | grep -v ".ref.c" | grep -v ".test.c"`; do
	correctness_check_file "$i" "$poccoptions" "$poccoptionsname" "$GCC_PERF" "$GCC_COMP_VER" "$mode";
	correctness_check_file "$i" "$poccoptions" "$poccoptionsname" "$ICC_PERF" "$ICC_COMP_VER" "$mode";
	if [ $RETVAL -eq 1 ]; then
	    ALLRETVAL=1;
	    ALLTESTSVAL=1;
	fi;
    done;
    if [ $ALLRETVAL -eq 0 ]; then
	echo "\033[32m[PASS][$mode] pocc $poccopts\033[0m";
    else
	echo "\033[31m[FAIL][$mode] pocc $poccopts\033[0m";
    fi;
}

################################################################################
################################################################################

echo "[PoCC-checker] Correctness+performance checker with $TESTSUITE_NAME";

if [ $# -lt 1 ]; then
    echo;
    echo "Usage: checker_perfcorrect.sh [mode] <pocc-options>";
    echo;
    echo "[mode] is one of:";
    echo "      correctness-small: test correctness only on the main configs."
    echo "      correctness: test correctness only on most configs."
    echo "      performance-small: test performance only on the main configs."
    echo "      performance: test performance only on most configs."
    echo "      correctness+performance-small: test correctness + performance only on the";
    echo "                                     main configs.";
    echo "      correctness+performance: test correctness + performance on most configs.";
    echo;
    echo "<pocc-options> (optional) is:";
    echo "      user-specifed configuration (set of pocc flags)";
    exit 1;
fi;

checkout_polybench;
if ! [ -f "$TRANSFORMER_COMMAND" ]; then
    echo "[PoCC-checker] Cannot find the pocc binary.";
    exit 1;
fi;

## Reset test files.
mkdir -p $FAILED_TESTS_DIR;
mkdir -p $PERF_TESTS_DIR;
ALLTESTSVAL=0;
curdate=`date "+%y-%m-%d-%T"`;
FAILED_TEST_FILE="$FAILED_TEST_FILE.$curdate";
PERF_FILE="$PERF_FILE_TEMPLATE.$curdate";
CSV_PERF_FILE="$CSV_PERF_FILE_TEMPLATE.$curdate";

# source icc.
$ICC_COMPILER_ENVSCRIPT;

## Temporarily unlimit stack size.
ulimit -s "unlimited";

MODE="$1";
userflags="$2";
small_only=`echo "$MODE" | grep small`;
correctness_only=`echo "$MODE" | grep correctness | grep -v performance`;
performance_only=`echo "$MODE" | grep performance | grep -v correctness`;

if ! [ -z "$userflags" ]; then
    correctness_performance_check_opts "$userflags" "user-specified" "$MODE";
else
    if ! [ -z "$small_only" ]; then
	## Execute the small test suite.
	while read confline; do
	    comment=`echo "$confline" | grep "#"`;
	    if [ -z "$comment" ]; then
		flags=`echo "$confline" | cut -d '|' -f 1`;
		label=`echo "$confline" | cut -d '|' -f 2 | sed -e "s/ //g"`;
		correctness_performance_check_opts "$flags" "label" "$MODE";
	    fi;
	done < $CONFLIST_SMALL;
    else
	## Execute the full test suite.
	while read confline; do
	    comment=`echo "$confline" | grep "#"`;
	    if [ -z "$comment" ]; then
		flags=`echo "$confline" | cut -d '|' -f 1`;
		label=`echo "$confline" | cut -d '|' -f 2 | sed -e "s/ //g"`;
		correctness_performance_check_opts "$flags" "label" "$MODE";
	    fi;
	done < $CONFLIST_LARGE;
    fi;
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

echo "\033[33m[Checker]\033[0m Computing data...";

releaseuid="";
if [ -z "$correctness_only" ]; then
    ### FIXME: do a per-module revision list. For the moment assume
    ### pocc-util upgrade (or svn up on pocc/) has been run.
    dirname=`dirname $TRANSFORMER_COMMAND`;
    releaseuid=`cd $dirname && svn info | grep Revision | cut -d : -f 2 | cut -d ' ' -f 2`;
    releaseuid="svnrev=$releaseuid";
    ## Prepare the csv data for the performance file.
    ./scripts/data_to_csv.sh "$PERF_FILE.$ICC_STRING_NAME" "$CSV_PERF_FILE.$releaseuid.$GCC_STRING_NAME.csv";
    ./scripts/data_to_csv.sh "$PERF_FILE.$ICC_STRING_NAME" "$CSV_PERF_FILE.$releaseuid.$ICC_STRING_NAME.csv";
   ## Compute the regression file.
    rm -f regressions.dat;
    ./scripts/compute_regressions.sh "$CSV_PERF_FILE.$releaseuid.$GCC_STRING_NAME.csv" "regressions.dat" "$PERF_TESTS_DIR" "$REGRESSION_THRESOLD";
    ./scripts/compute_regressions.sh "$CSV_PERF_FILE.$releaseuid.$ICC_STRING_NAME.csv" "regressions.dat" "$PERF_TESTS_DIR" "$REGRESSION_THRESOLD";
fi;

## Send email with the results.
echo "[PoCC] Correctness-performance checker: all finished on `hostname` at `date`" > email.out
echo >> email.out;
echo "------------------------------------------------------------------------------" >> email.out;
echo >> email.out;
if [ -z "$performance_only" ]; then
    echo "* Failed tests: " >> email.out;
    echo >> email.out;
    if [ -f "$FAILED_TEST_FILE" ]; then
	cat $FAILED_TEST_FILE >> email.out;
    else
	echo "-- all tests passed --" >> email.out;
    fi;
    echo >> email.out;
    echo "------------------------------------------------------------------------------" >> email.out;
    echo >> email.out;
fi;
if [ -z "$correctness_only" ]; then
    echo "* Performance regressions/improvements:" >> email.out;
    echo >> email.out;
    if [ -f "regressions.dat" ]; then
	cat regressions.dat >> email.out;
    else
	echo "-- no regression --" >> email.out;
    fi;
    echo >> email.out;
    echo "------------------------------------------------------------------------------" >> email.out;
    echo >> email.out;
    echo "* GCC Performance results ($GCC_STRING_NAME):" >> email.out
    echo >> email.out;
    cat $CSV_PERF_FILE.$releaseuid.$GCC_STRING_NAME.csv >> email.out;
    echo >> email.out;
    echo "------------------------------------------------------------------------------" >> email.out;
    echo >> email.out;
    echo "* ICC Performance results ($ICC_STRING_NAME):" >> email.out
    echo >> email.out;
    cat $CSV_PERF_FILE.$releaseuid.$ICC_STRING_NAME.csv >> email.out;
    echo >> email.out;
    echo "------------------------------------------------------------------------------" >> email.out;
    echo >> email.out;
fi;
cat email.out | mail -s "PoCC experiments finished" "$EMAIL_MAINTAINER";
rm -f email.out regressions.dat;

echo "\033[33m[Checker]\033[0m Summary email send to $EMAIL_MAINTAINER";
echo "\033[33m[Checker]\033[0m Failed test database updated: $FAILED_TEST_FILE";
if [ -z "$correctness_only" ]; then
    echo "\033[33m[Checker]\033[0m Performance test database updated: $CSV_PERF_FILE.$releaseuid.$GCC_STRING_NAME.csv";
    echo "\033[33m[Checker]\033[0m Performance test database updated: $CSV_PERF_FILE.$releaseuid.$ICC_STRING_NAME.csv";
fi;
echo "\033[33m[Checker]\033[0m All done";
