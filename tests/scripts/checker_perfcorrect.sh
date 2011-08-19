#!/bin/sh
## checker_correctness.sh for PoCC in /home/pouchet/osu/projects/rice/PACE-code/trunk/polyrose
##
## Made by Louis-Noel Pouchet
## Contact: <pouchet@cse.ohio-state.edu>
##
## Started on  Tue Jul 12 14:34:28 2011 Louis-Noel Pouchet
## Last update Tue Aug  9 16:44:38 2011 Louis-Noel Pouchet
##

################################################################################
################################################################################
## Global variables (user-defined).
## Email address of the receiver of the script result.
EMAIL_MAINTAINER="polyhedral-automated@cse.ohio-state.edu";
## GCC command line for the performance check.
GCC_COMPILER_COMMAND="gcc -O3 -fopenmp";
## String to identify GCC version used for performance. No space.
GCC_STRING_NAME="gcc-4.3";
## Extra compiler flags for the correctness check, if any.
GCC_CHECK_EXTRA_FLAGS="";
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
## Program command to generate the transformed programs. Set to "" to not use
## a tool to generate the transformed variants (they are already present).
TRANSFORMER_COMMAND="../driver/src/pocc";
## Default option(s) applied to all programs.
TRANSFORMER_DEFAULT_OPTS="--quiet";
## Regression/improvement thresold. Here, 10%.
REGRESSION_THRESOLD=10;
## Timeout for the transformed program. Here, 10 minutes.
PROGRAM_TIMEOUT=600;
## Maximal variance accepted between the 3 median runs for performance results.
## Here 10%
VARIANCE_ACCEPTED=10;


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
FAILED_TEST_FILE="$FAILED_TESTS_DIR/failed.tests";
PERF_TESTS_DIR="perf-tests";
PERF_FILE_TEMPLATE="$PERF_TESTS_DIR/perf.out"
CSV_PERF_FILE_TEMPLATE="$PERF_TESTS_DIR/perf"
GCC_CORRECT="gcc -O0 -fopenmp -lm -DPOLYBENCH_DUMP_ARRAYS $GCC_CHECK_EXTRA_FLAGS";
POLYBENCH_PERF_FLAGS="-lm -DPOLYBENCH_TIME -DPOLYBENCH_CACHE_SIZE_KB=$LLC_CACHE_SIZE";
GCC_PERF="$GCC_COMPILER_COMMAND $POLYBENCH_PERF_FLAGS";
GCC_COMP_VER="$GCC_STRING_NAME";
ICC_PERF="$ICC_COMPILER_COMMAND $POLYBENCH_PERF_FLAGS";
ICC_COMP_VER="$ICC_STRING_NAME";
COMP_OTHERS=" -I $TESTSUITE_NAME/utilities $TESTSUITE_NAME/utilities/instrument.c";
################################################################################
################################################################################

## Find the echo color command, if any.
#if test -f "/bin/echo"; then ECHO_CMD="/bin/echo"; else ECHO_CMD="echo"; fi;
ECHO_CMD="echo";
test_echo=`$ECHO_CMD -e "toto"`;
if [ "$test_echo" = "-e toto" ]; then
    ECHO_CMD="$ECHO_CMD";
else
    ECHO_CMD="$ECHO_CMD -e";
fi;

check_error()
{
    if [ $1 -ne 0 ]; then
	$ECHO_CMD "Error executing $2";
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
    benchcomputed="$2";
    cat "$file" | grep "[0-9]\+" | sort -n | head -n 4 | tail -n 3 > avg.out;
    expr="(0";
    while read n; do
	expr="$expr+$n";
    done < avg.out;
    time=`echo "scale=8;$expr)/3" | bc`;
    tmp=`echo "$time" | cut -d '.' -f 1`;
    if [ -z "$tmp" ]; then
	time="0$time";
    fi;
    val1=`cat avg.out | head -n 1`;
    val2=`cat avg.out | head -n 2 | tail -n 1`;
    val3=`cat avg.out | head -n 3 | tail -n 1`;
    val11=`echo "a=$val1 - $time;if(0>a)a*=-1;a" | bc`;
    val12=`echo "a=$val2 - $time;if(0>a)a*=-1;a" | bc`;
    val13=`echo "a=$val3 - $time;if(0>a)a*=-1;a" | bc`;
    myvar=`echo "$val11 $val12 $val13" | awk '{ if ($1 > $2) { if ($1 > $3) print $1; else print $3; } else { if ($2 > $3) print $2; else print $3; } }'`;
    variance=`echo "scale=5;($myvar/$time)*100" | bc`;
    tmp=`echo "$variance" | cut -d '.' -f 1`;
    if [ -z "$tmp" ]; then
	variance="0$variance";
    fi;
    compvar=`echo "$variance $VARIANCE_ACCEPTED" | awk '{ if ($1 < $2) print "ok"; else print "error"; }'`;
    if [ "$compvar" = "error" ]; then
	$ECHO_CMD "\033[31m[WARNING]\033[0m Variance is above thresold, unsafe performance measurement";
	$ECHO_CMD "        => max deviation=$variance%, tolerance=$VARIANCE_ACCEPTED%";
	WARNING_VARIANCE="$WARNING_VARIANCE\n$benchcomputed: max deviation=$variance%, tolerance=$VARIANCE_ACCEPTED%";
    else
	echo "[INFO] Maximal deviation from arithmetic mean of 3 average runs: $variance%";
    fi;
    PROCESSED_TIME="$time";
    rm -f avg.out;
}


exec_timeout_prog()
{
    executable="$1";
    progoutputtrace="$2";
    ret=`perl -e 'alarm shift @ARGV; exec @ARGV' $PROGRAM_TIMEOUT $executable >> $progoutputtrace 2>/dev/null`;
    if [ $? -ne 0 ]; then
	echo "error" >> $progoutputtrace;
	RETURN_EXEC="error: timeout";
    else
	RETURN_EXEC="$ret";
    fi;
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
    perfretval=1;
    echo "[CHECK] Testing $filename with $TRANSFORMER_COMMAND $poccopts";
    if [ -z "$TRANSFORMER_COMMAND" ]; then
	if ! [ test -f "$filename.$poccoptsname.test.c" ]; then
	    echo "[ERROR] File $filename.$poccoptsname.test.c does not exist";
	    return;
	fi;
    else
	output=`$TRANSFORMER_COMMAND $TRANSFORMER_DEFAULT_OPTS $poccopts $filename -o $filename.$poccoptsname.test.c`;
	if [ $? -ne 0 ]; then
	    echo "$output";
	    $ECHO_CMD "\033[31m[FAIL][Correctness] $filename ($poccopts)\033[0m";
	    mv $filename.$poccoptsname.test.c $FAILED_TESTS_DIR;
	    echo "$filename | $TRANSFORMER_COMMAND $TRANSFORMER_DEFAULT_OPTS $poccopts" >> $FAILED_TEST_FILE;
	    return;
	fi;
    fi;
    if [ -z "$performance_only" ]; then
	if [ -f $filename.$poccoptsname.ref.c ]; then
	    diff=`diff $filename.$poccoptsname.ref.c $filename.$poccoptsname.test.c`;
	    if [ -z "$diff" ]; then
		$ECHO_CMD "\033[32m[PASS][Correctness]\033[0m $filename ($poccopts)";
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
		$ECHO_CMD "\033[32m[PASS][Correctness]\033[0m $filename ($poccopts)";
		mv $filename.$poccoptsname.test.c $filename.$poccoptsname.ref.c
		rm -f $FAILED_TESTS_DIR/$filename.$poccoptsname.test.c
		RETVAL=0;
	    else
		$ECHO_CMD "\033[31m[FAIL][Correctness] $filename ($poccopts)\033[0m";
		mv $filename.$poccoptsname.test.c $FAILED_TESTS_DIR;
		echo "$filename | $TRANSFORMER_COMMAND $TRANSFORMER_DEFAULT_OPTS $poccopts" >> $FAILED_TEST_FILE;
	    fi;
	fi;
    fi;
    if [ -z "$correctness_only" ]; then
	if [ -f $filename.$poccoptsname.ref.c ] &&
	    [ -f $filename.$poccoptsname.ref.c.time.$comp_ver ]; then
	    diff=`diff $filename.$poccoptsname.ref.c $filename.$poccoptsname.test.c`;
	    if [ -z "$diff" ]; then
		$ECHO_CMD "\033[32m[No change]\033[0m $filename ($poccopts)";
		rm -f $FAILED_TESTS_DIR/$filename.$poccoptsname.test.c
		time=`cat $filename.$poccoptsname.ref.c.time.$comp_ver`;
		echo "$filename.$poccoptsname | $time" >> $PERF_FILE.$comp_ver;
		RETVAL=0;
		perfretval=0;
	    fi;
	fi;
	if [ $perfretval -eq 1 ]; then
	    echo "[CHECK] Checking performance of outputs...";
	    if [ -f $filename.$poccoptsname.ref.c ]; then
		cp $filename.$poccoptsname.ref.c $filename.$poccoptsname.test.c;
	    fi;
	    rm -f test;
	    out=`$comp_perf $COMP_OTHERS $filename.$poccoptsname.test.c -o test  >/dev/null 2>/dev/null`;
	    if [ $? -ne 0 ]; then
		$ECHO_CMD "\033[31m[FAIL][Performance] $filename ($poccopts) with $comp_ver\033[0m";
		return;
	    fi;
	    rm -f $filename.$poccoptsname.ref.c.time.$comp_ver;
	    # 5 runs.
	    exec_timeout_prog "./test" "$filename.$poccoptsname.ref.c.time.$comp_ver";
	    exec_timeout_prog "./test" "$filename.$poccoptsname.ref.c.time.$comp_ver";
	    exec_timeout_prog "./test" "$filename.$poccoptsname.ref.c.time.$comp_ver";
	    exec_timeout_prog "./test" "$filename.$poccoptsname.ref.c.time.$comp_ver";
	    exec_timeout_prog "./test" "$filename.$poccoptsname.ref.c.time.$comp_ver";
	    has_error=`grep "error" "$filename.$poccoptsname.ref.c.time.$comp_ver"`;
	    if ! [ -z "$has_error" ]; then
		echo "OUTPUT with $comp_ver: $ret";
		$ECHO_CMD "\033[31m[FAIL][Performance] $filename ($poccopts)\033[0m";
		mv $filename.$poccoptsname.test.c $FAILED_TESTS_DIR;
		echo "$filename | $TRANSFORMER_COMMAND $TRANSFORMER_DEFAULT_OPTS $poccopts" >> $FAILED_TEST_FILE;
		return;
	    fi;
	    compute_mean_exec_time "$filename.$poccoptsname.ref.c.time.$comp_ver" "$filename ($poccoptsname)";
	    echo "$filename.$poccoptsname | $PROCESSED_TIME" >> $PERF_FILE.$comp_ver;
	    echo "$PROCESSED_TIME" > $filename.$poccoptsname.ref.c.time.$comp_ver;
	    $ECHO_CMD "\033[32m[PASS][Performance]\033[0m Time: $PROCESSED_TIME | $filename ($poccopts)";
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
    correctness_only=`echo "$mode" | grep correctness | grep -v performance`;
    $ECHO_CMD "\033[33m[Checker]\033[0m Testing $TRANSFORMER_COMMAND $TRANSFORMER_DEFAULT_OPTS";
    ALLRETVAL=0;
    RETVAL=0;
    if [ -z "$TRANSFORMER_COMMAND" ]; then
	## Scan all files, as we do not use a transformer to generate the code.
	for i in `find $TESTSUITE_NAME -name "*.c" | grep -v utilities`; do
	    correctness_check_file "$i" "$poccoptions" "$poccoptionsname" "$GCC_PERF" "$GCC_COMP_VER" "$mode";
	    if [ -z "$correctness_only" ]; then
		correctness_check_file "$i" "$poccoptions" "$poccoptionsname" "$ICC_PERF" "$ICC_COMP_VER" "$mode";
	    fi;
	    if [ $RETVAL -eq 1 ]; then
		ALLRETVAL=1;
		ALLTESTSVAL=1;
	    fi;
	done;
    else
	## Scan only non-generated files.
	for i in `find $TESTSUITE_NAME -name "*.c" | grep -v utilities | grep -v ".ref.c" | grep -v ".test.c"`; do
	    correctness_check_file "$i" "$poccoptions" "$poccoptionsname" "$GCC_PERF" "$GCC_COMP_VER" "$mode";
	    if [ -z "$correctness_only" ]; then
		correctness_check_file "$i" "$poccoptions" "$poccoptionsname" "$ICC_PERF" "$ICC_COMP_VER" "$mode";
	    fi;
	    if [ $RETVAL -eq 1 ]; then
		ALLRETVAL=1;
		ALLTESTSVAL=1;
	    fi;
	done;
    fi;
    if [ $ALLRETVAL -eq 0 ]; then
	$ECHO_CMD "\033[32m[PASS][$mode] pocc $poccopts\033[0m";
    else
	$ECHO_CMD "\033[31m[FAIL][$mode] pocc $poccopts\033[0m";
    fi;
}


data_to_csv()
{
    ## a benchmark always in the test suite.
    data_perf_file="$1";
    OUTPUT_CSV="$2";
    CONFIG_LIST_FILE="$3";

    rm -f conf.list;
    while read nnn; do
	conf=`echo "$nnn" | cut -d '|' -f 2 | cut -d ' ' -f 2`;
	echo "$conf" >> conf.list;
    done < $CONFIG_LIST_FILE;

    confs="#";
    while read nnn; do
	confs="$confs $nnn";
    done < conf.list;
    echo "$confs" > $OUTPUT_CSV;
    while read nnn; do
	cat $data_perf_file | grep "\.$nnn |" > file.list;
	while read iii; do
	    filen=`echo "$iii" | cut -d '|' -f 1 | xargs basename | cut -d '.' -f 1`;
	    etime=`echo "$iii" | cut -d '|' -f 2`;
	    missing=`echo "$etime" | grep ".c."`;
	    if ! [ -z "$missing" ]; then
		etime="-1";
	    fi;
	    firstd=`grep "$filen " $OUTPUT_CSV | grep -v "#"`;
	    if [ -z "$firstd" ]; then
		echo "$filen $etime" >> $OUTPUT_CSV;
	    else
		newcsv=`sed -e "s/\($filen .*\)/\1 $etime/g" $OUTPUT_CSV`;
		echo "$newcsv" > $OUTPUT_CSV;
	    fi;
	done < file.list;
	rm -f file.list
    done < conf.list;
    rm -f conf.list;
}

find_best_time()
{
    bench="$1";
    conf="$2";
    csvfilelist="$3";
    output="";
    best_ver="";
    best_id="";
    while read csvfile; do
	# find col.
 	confs=`head -n 1 "$csvfile" | cut -d '#' -f 2 | sed -e "s/ [ ]*/ /g"`;
	count2=2;
	test=`echo "$confs" | cut -d ' ' -f "$count2"`;
	is_unique=`echo "$confs" | sed -e "s/ //g"`;
	if [ "$is_unique" != "$test" ]; then
	    while [ "$test" != "$conf" ] && ! [ -z "$test" ];  do
		count2=$(($count2+1));
		test=`echo "$confs" | cut -d ' ' -f "$count2"`;
	    done;
	fi;
	if ! [ -z "$test" ]; then
	    output=`head -n 1 | grep "$bench " "$csvfile" | sed -e "s/ [ ]*/ /g" | cut -d ' ' -f $count2`;
	    if [ -z "$best_ver" ]; then
		best_ver="$output";
		best_id="$csvfile";
	    else
		comp=`echo "$output $best_ver" | awk '{if ($1 < $2) print $1; else print $2}'`;
		if [ "$comp" = "$output" ]; then
		    best_id="$csvfile";
		    best_ver="$comp";
		fi;
	    fi;
	fi;
    done < $csvfilelist;
    BEST_TIME="$best_ver";
    BEST_ID="$best_id";
}

compute_regressions()
{
    input_file="$1";
    output_file="$2";
    perf_test_dir="$3";
    thresold="$4";
    rm -f $output_file;
    conflist=`head -n 1 $input_file`;
    conflist=`echo "$conflist" | sed -e "s/#//g"`;
    count=2;
    rm -f regressions.tmp.dat;
    rm -f improvements.tmp.dat;
    rm -f $output_file;
    cat $input_file | grep -v "#" > file.list;
    find $perf_test_dir -name "*.csv" > csvfile.list
    for i in $conflist; do
        ## iterate on all files.
	while read n; do
	    bench=`echo "$n" | cut -d ' ' -f 1`;
	    curtime=`echo "$n" | sed -e "s/ [ ]*/ /g" | cut -d ' ' -f $count`;
	    is_numerical=`echo "$curtime" | grep "^[0-9\.]\+$"`;
	    if [ "$is_numerical" != "$curtime" ]; then
		curtime="error";
	    fi;
	    find_best_time "$bench" "$i" "csvfile.list";
	    if [ "$curtime" != "error" ] && ! [ -z "$BEST_TIME" ]; then
		comp=`echo "scale=2;(1-$curtime/$BEST_TIME)*100" | bc`;
		compnorm=`echo "$comp" | sed -e "s/-//g"`;
		reg=`echo "$compnorm $thresold" | awk '{if ($1 > $2) print "difference"; else print $1}'`;
		if [ "$reg" = "difference" ]; then
		    is_dec=`echo "$comp" | grep "-"`;
		    verid=`echo "$BEST_ID" | cut -d '=' -f 2 | cut -d '.' -f 1`;
		    normalized=`echo "$compnorm" | cut -d '.' -f 1`;
		    if [ -z "$normalized" ]; then normalized="0$compnorm"; fi;
		    if ! [ -z "$is_dec" ]; then
			echo "$bench w/ $conf: regression (+$normalized%), from Rev. $verid" >> regressions.tmp.dat;
		    else
			echo "$bench w/ $conf: improvement (-$normalized%), from Rev. $verid" >> improvements.tmp.dat;
		    fi;
		fi;
	    else
		if ! [ -z "$BEST_TIME" ]; then
		    verid=`echo "$BEST_ID" | cut -d '=' -f 2 | cut -d '.' -f 1`;
		    echo "$bench w/ $conf: regression (failed execution), from Rev. $verid" >> regressions.tmp.dat;
		fi;
	    fi;
	done < file.list;
	count=$(($count+1));
    done;
    if [ -f "regressions.tmp.dat" ]; then
	echo "** Regressions **" >> $output_file;
	cat regressions.tmp.dat >> $output_file;
	echo >> $output_file;
    fi;
    if [ -f "improvements.tmp.dat" ]; then
	echo "** Improvements **" >> $output_file;
	cat improvements.tmp.dat >> $output_file;
	echo >> $output_file;
    fi;
    rm -f file.list;
    rm -f csvfile.list;
    rm -f regressions.tmp.dat;
    rm -f improvements.tmp.dat;
}

################################################################################
################################################################################

echo "[Checker] Correctness+performance checker with $TESTSUITE_NAME";

if [ $# -ne 2 ]; then
    echo "Usage: performance_checker.sh [mode] [configs]";
    echo;
    echo "[mode] is one of:";
    echo "      correctness: test correctness only, on [configs]"
    echo "      performance: test performance only on most [configs]"
    echo "      correctness+performance: test correctness + performance on [configs]";
    echo;
    echo "[configs] is a text file, with one line per configuration to test.";
    echo;
    exit 1;
fi;

if ! [ -f "$TRANSFORMER_COMMAND" ] && ! [ -z "$TRANSFORMER_COMMAND" ]; then
    echo "[Checker] Cannot find the transformer binary.";
    exit 1;
fi;

## Get the test suite.
checkout_polybench;

## Reset test files.
mkdir -p $FAILED_TESTS_DIR;
mkdir -p $PERF_TESTS_DIR;
ALLTESTSVAL=0;
curdate=`date "+%y-%m-%d-%T"`;
FAILED_TEST_FILE="$FAILED_TEST_FILE.$curdate";
PERF_FILE="$PERF_FILE_TEMPLATE.$curdate";
CSV_PERF_FILE="$CSV_PERF_FILE_TEMPLATE.$curdate";
WARNING_VARIANCE="";

# source icc.
$ICC_COMPILER_ENVSCRIPT;

## Temporarily unlimit stack size.
ulimit -s "unlimited";

MODE="$1";
CONFLIST="$2";
correctness_only=`echo "$MODE" | grep correctness | grep -v performance`;
performance_only=`echo "$MODE" | grep performance | grep -v correctness`;

START_DATE=`date +%s`;
while read confline; do
    comment=`echo "$confline" | grep "#"`;
    if [ -z "$comment" ]; then
	flags=`echo "$confline" | cut -d '|' -f 1`;
	label=`echo "$confline" | cut -d '|' -f 2 | sed -e "s/ //g"`;
	correctness_performance_check_opts "$flags" "$label" "$MODE";
    fi;
done < $CONFLIST;
STOP_DATE=`date +%s`;
TOTAL_SCRIPT_TIME=`date -v-19H -r $(($STOP_DATE-$START_DATE)) +%T`;

if [ $ALLTESTSVAL -eq 0 ]; then
    $ECHO_CMD "\033[32m[PASS] Correctness checker\033[0m";
else
    if [ -f "$FAILED_TEST_FILE" ]; then
	$ECHO_CMD "\033[31m[FAIL] Failed test(s) summary\033[0m";
	while read n; do
	    $ECHO_CMD "\033[31m[FAIL] $n\033[0m";
	done < $FAILED_TEST_FILE;
    fi;
    $ECHO_CMD "\033[31m[FAIL] Correctness checker\033[0m";
fi;

$ECHO_CMD "\033[33m[Checker]\033[0m Computing data...";

releaseuid="";
if [ -z "$correctness_only" ]; then
    ### FIXME: do a per-module revision list. For the moment assume
    ### pocc-util upgrade (or svn up on pocc/) has been run.
    dirname=`dirname $TRANSFORMER_COMMAND`;
    releaseuid=`cd $dirname && svn info | grep Revision | cut -d : -f 2 | cut -d ' ' -f 2`;
    releaseuid="svnrev=$releaseuid";
    ## Prepare the csv data for the performance file.
    if [ -f $PERF_FILE.$GCC_STRING_NAME ]; then
	data_to_csv "$PERF_FILE.$GCC_STRING_NAME" "$CSV_PERF_FILE.$releaseuid.$GCC_STRING_NAME.csv" "$CONFLIST";
    fi;
    if [ -f $PERF_FILE.$ICC_STRING_NAME ]; then
	data_to_csv "$PERF_FILE.$ICC_STRING_NAME" "$CSV_PERF_FILE.$releaseuid.$ICC_STRING_NAME.csv" "$CONFLIST";
    fi;
   ## Compute the regression file.
    rm -f regressions.dat;
    if [ -f "$CSV_PERF_FILE.$releaseuid.$GCC_STRING_NAME.csv" ]; then
	compute_regressions "$CSV_PERF_FILE.$releaseuid.$GCC_STRING_NAME.csv" "regressions.dat" "$PERF_TESTS_DIR" "$REGRESSION_THRESOLD";
    fi;
    if [ -f "$CSV_PERF_FILE.$releaseuid.$ICC_STRING_NAME.csv" ]; then
	compute_regressions "$CSV_PERF_FILE.$releaseuid.$ICC_STRING_NAME.csv" "regressions.dat" "$PERF_TESTS_DIR" "$REGRESSION_THRESOLD";
    fi;
fi;

## Send email with the results.
echo "To: polyhedral-automated@cse.ohio-state.edu" > email.out;
echo "Subject: $TRANSFORMER_COMMAND correctness+performance status report" >> email.out;
echo "[Checker] Correctness-performance checker: all finished on `hostname` on `date` $releaseuid" >> email.out;
echo "[Checker] Total testing time: $TOTAL_SCRIPT_TIME" >> email.out;
echo >> email.out;
echo "------------------------------------------------------------------------------" >> email.out;
echo >> email.out;
echo "* Tested configurations:" >> email.out;
echo >> email.out;
while read n; do
    flags=`echo "$n" | cut -d '|' -f 1`;
    label=`echo "$n" | cut -d '|' -f 2`;
    echo "Configuration: \"$label \" uses $TRANSFORMER_COMMAND $TRANSFORMER_DEFAULT_OPTS $flags" >> email.out;
done < $CONFLIST;
echo >> email.out;
echo "------------------------------------------------------------------------------" >> email.out;
echo >> email.out;
if [ -z "$correctness_only" ] && ! [ -z "$WARNING_VARIANCE" ]; then
    echo "[WARNING] Some performance data exceeded the authorized variance." >> email.out;
    echo "[WARNING] Performance results cannot be trusted, unsafe environment." >> email.out;
    echo >> email.out;
    echo "* Problems observed: " >> email.out;
    echo "$WARNING_VARIANCE" >> email.out;
    echo >> email.out;
    echo "------------------------------------------------------------------------------" >> email.out;
    echo >> email.out;
fi;
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
    if [ -f "$CSV_PERF_FILE.$releaseuid.$GCC_STRING_NAME.csv" ]; then
	cat $CSV_PERF_FILE.$releaseuid.$GCC_STRING_NAME.csv >> email.out;
    else
	echo "-- ERROR generating $CSV_PERF_FILE.$releaseuid.$GCC_STRING_NAME.csv --" >> email.out;
    fi;
    echo >> email.out;
    echo "------------------------------------------------------------------------------" >> email.out;
    echo >> email.out;
    echo "* ICC Performance results ($ICC_STRING_NAME):" >> email.out
    echo >> email.out;
    if [ -f "$CSV_PERF_FILE.$releaseuid.$ICC_STRING_NAME.csv" ]; then
	cat $CSV_PERF_FILE.$releaseuid.$ICC_STRING_NAME.csv >> email.out;
    else
	echo "-- ERROR generating $CSV_PERF_FILE.$releaseuid.$ICC_STRING_NAME.csv --" >> email.out;
    fi;
    echo >> email.out;
    echo "------------------------------------------------------------------------------" >> email.out;
    echo >> email.out;
fi;

## Print email.
$ECHO_CMD "\033[33m[Checker]\033[0m Summary email to be sent to $EMAIL_MAINTAINER:";
cat email.out;

## Send email.
#cat email.out | mail -s "$TRANSFORMER_COMMAND experiments finished" "$EMAIL_MAINTAINER" -- -r "$EMAIL_MAINTAINER";
cat email.out | sendmail -f "pouchet@cse.ohio-state.edu" -t "$EMAIL_MAINTAINER"  "$EMAIL_MAINTAINER";
rm -f email.out regressions.dat;
$ECHO_CMD "\033[33m[Checker]\033[0m Summary email sent to $EMAIL_MAINTAINER";

## Stdout summary.
if [ -f $FAILED_TEST_FILE ]; then
    $ECHO_CMD "\033[33m[Checker]\033[0m Failed test database updated: $FAILED_TEST_FILE";
fi;
if [ -z "$correctness_only" ]; then
    $ECHO_CMD "\033[33m[Checker]\033[0m Performance test database updated: $CSV_PERF_FILE.$releaseuid.$GCC_STRING_NAME.csv";
    $ECHO_CMD "\033[33m[Checker]\033[0m Performance test database updated: $CSV_PERF_FILE.$releaseuid.$ICC_STRING_NAME.csv";
fi;
$ECHO_CMD "\033[33m[Checker]\033[0m All done";
