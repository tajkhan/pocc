#!/bin/sh
## installer-svn.sh<2> for pocc in /Users/pouchet
##
## Made by Louis-Noel Pouchet
## Contact: <louis-noel.pouchet@inria.fr>
##
## Started on  Mon Apr  6 01:03:01 2009 Louis-Noel Pouchet
## Last update Mon Apr  6 02:35:42 2009 Louis-Noel Pouchet
##

config_opts=""
install_dir="`pwd`/install"
errors="";
nl='
';

check_error()
{
    if [ $1 -ne 0 ]; then
	echo "[Error] $2";
	errors="$errors$nl$[Error] $2";
	exit 1;
    fi;
}

install_version()
{
    dir="$1";
    soft="$2";
    config_create_cmd="$3";
    if [ -z "$config_create_cmd" ]; then
	config_create_cmd=";";
    fi;
    if [ "$4" = "no-subdir" ]; then
	br="$dir";
    else
	br=`find $dir -maxdepth 1 | grep -v svn | cut -d '/' -f 2 | grep -v $dir`;
    fi;
    for v in $br; do
	versions="$versions$nl$v";
	if ! [ -d "$install_dir/$v" ]; then
	    mkdir -p $install_dir/$v;
	fi;
	if [ "$4" = "no-subdir" ]; then
	    cddir="$v";
	else
	    cddir="$dir/$v";
	fi;
	r=0;
	cd $cddir && \
	    if ! [ -f configure ]; then $config_create_cmd; fi; \
	    if ! [ -f config.stamp ]; then \
	    ./configure --prefix=$install_dir/$v $config_opts; r=$?; fi; cd -;
	check_error $r "$soft configuration ($v)";
	if [ $r -ne 0 ]; then continue; fi;
	touch $cddir/config.stamp;
	cd $cddir && make; r=$?; cd -;
	check_error $r "$soft make ($v)";
	if [ $r -ne 0 ]; then continue; fi;
	cd $cddir && make install; r=$?; cd -
	check_error $r "$soft make install ($v)";
    done;
}

main()
{
    versions="";
    soft="$1";
    config_create_cmd="$2";
    install_version "branches" "$soft" "$config_create_cmd"
    install_version "tags" "$soft" "$config_create_cmd"
    install_version "trunk" "$soft" "$config_create_cmd" "no-subdir"
    echo "--- $soft Errors ---"
    echo "$errors";
    echo "--- $soft installed versions ---"
    echo "$versions";
}

main "$@";
