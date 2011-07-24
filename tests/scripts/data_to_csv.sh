#!/bin/sh
## mkdir data_to_csv.sh for  in /Users/pouchet
##
## Made by Louis-Noel Pouchet
## Contact: <pouchet@cse.ohio-state.edu>
##
## Started on  Fri Jul 22 17:58:30 2011 Louis-Noel Pouchet
## Last update Sun Jul 24 00:09:01 2011 Louis-Noel Pouchet
##


if [ $# -ne 2 ]; then exit 1; fi;

## a benchmark always in the test suite.
BASEFILE="3mm"
PERF_FILE="$1";
OUTPUT_CSV="$2";

grep $BASEFILE $PERF_FILE | cut -d '|' -f 1 | xargs basename | sed -e "s/.*\.c\.\(.*\)/\1/g" > conf.list
confs="#";
while read n; do
    confs="$confs $n";
done < conf.list;
echo "$confs" > $OUTPUT_CSV;
while read n; do
    cat $PERF_FILE | grep "\.$n |" > file.list;
    while read i; do
	file=`echo "$i" | cut -d '|' -f 1 | xargs basename | cut -d '.' -f 1`;
	time=`echo "$i" | cut -d '|' -f 2`;
	missing=`echo "$time" | grep ".c."`;
	if ! [ -z "$missing" ]; then
	    time="-1";
	fi;
	first=`grep "$file " $OUTPUT_CSV | grep -v "#"`;
	if [ -z "$first" ]; then
	    echo "$file $time" >> $OUTPUT_CSV;
	else
	    newcsv=`sed -e "s/\($file .*\)/\1 $time/g" $OUTPUT_CSV`;
	    echo "$newcsv" > $OUTPUT_CSV;
	fi;
    done < file.list;
    rm -f file.list
done < conf.list;
rm -f conf.list
