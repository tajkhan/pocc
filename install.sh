#!/bin/sh
## install.sh for pocc in /Users/pouchet
## 
## Made by Louis-Noel Pouchet
## Contact: <louis-noel.pouchet@inria.fr>
## 
## Started on  Mon Apr  6 02:01:54 2009 Louis-Noel Pouchet
## Last update Mon Apr  6 02:49:05 2009 Louis-Noel Pouchet
##

dirs="
math/fm
math/piplib
math/polylib
analyzers/clan
analyzers/candl
generators/cloog
generators/pace
optimizers/letsee
";


# config_file
config_file="config/pocc-install.cfg"

svn_checkout()
{
    soft="$1";
    full="$2";
    svnrepos=`grep $1 $config_file | cut -d ' ' -f 3`;
    svn co $svnrepos $2;
}

config_cmd()
{
    soft="$1";
    full="$2";
    confcmd=`grep $1 $config_file | cut -d ' ' -f 4`;
}

get_type()
{
    soft="$1";
    type=`grep $1 $config_file | cut -d ' ' -f 2`;
}

retrieve_code()
{
    soft="$1";
    full="$2";
    type="";
    get_type $soft;
    case $type in
	svn) svn_checkout $soft $full;;
	*) echo "Unsupported type";;
    esac;
}

installer()
{
    type="$1";
    soft="$2";
    conf="$3";
    case $type in
	svn) ../../config/installer-svn.sh $soft "$conf";;
	*) echo "Unsupported type";;
    esac;
}

for d in $dirs; do
    subd=`echo "$d" | cut -d '/' -f 2`;
    retrieve_code $subd $d;
    confcmd="";
    config_cmd $subd;
    cd $d && installer $type $subd $confcmd ; cd -;
    

done;
