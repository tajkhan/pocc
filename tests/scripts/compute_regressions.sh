#!/bin/sh
## compute_regressions.sh for  in /Users/pouchet
##
## Made by Louis-Noel Pouchet
## Contact: <pouchet@cse.ohio-state.edu>
##
## Started on  Sat Jul 23 21:40:54 2011 Louis-Noel Pouchet
## Last update Sun Jul 24 00:08:54 2011 Louis-Noel Pouchet
##

if [ $# -ne 4 ]; then exit 1; fi;

input_file="$1";
output_file="$2";
perf_test_dir="$3";
thresold="$4";
rm -f $output_file;

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
 	#confs=`head -n 1 "$csvfile" | sed -e "s/#//g" | sed -e "s/[ ]\+/ /g"`;
 	confs=`head -n 1 "$csvfile" | cut -d ' ' -f 2`;
	count2=2;
	test=`echo "$confs" | cut -d ' ' -f "$count2"`;
	while [ "$test" != "$conf" ] && ! [ -z "$test" ];  do
	    count2=$(($count2+1));
	    test=`echo "$confs" | cut -d ' ' -f "$count2"`;
	done;
	count2=$(($count2+1));
	if ! [ -z "$test" ]; then
	    output=`head -n 1 | grep "$bench " "$csvfile" | cut -d ' ' -f $count2`;
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

conflist=`head -n 1 $input_file`;
conflist=`echo "$conflist" | sed -e "s/#//g"`;
count=3;
rm -f regressions.tmp.dat;
rm -f improvements.tmp.dat;
rm -f $output_file;
cat $input_file | grep -v "#" > file.list;
find $perf_test_dir -name "*.csv" > csvfile.list
for i in $conflist; do
    ## iterate on all files.
    while read n; do
	bench=`echo "$n" | cut -d ' ' -f 1`;
	curtime=`echo "$n" | cut -d ' ' -f $count`;
	find_best_time "$bench" "$i" "csvfile.list";
	comp=`echo "scale=2;($BEST_TIME-$curtime)/$BEST_TIME" | bc`;
	compnorm=`echo "$comp" | sed -e "s/-//g"`;
	reg=`echo "$compnorm $thresold" | awk '{if ($1 > $2) print "difference"; else print $1}'`;
	if [ "$reg" = "difference" ]; then
	    is_dec=`echo "$comp" | grep "-"`;
	    verid=`echo "$BEST_ID" | cut -d '=' -f 2 | cut -d '.' -f 1`;
	    if ! [ -z "$is_dec" ]; then
		echo "$bench w/ $conf: regression ($comp%), from Rev. $verid" >> regressions.tmp.dat;
	    else
		echo "$bench w/ $conf: improvement ($comp%%), from Rev. $verid" >> improvements.tmp.dat;
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
