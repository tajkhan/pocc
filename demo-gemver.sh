#!/bin/sh
## demo-gemver.sh for gemver in /Users/pouchet
## 
## Made by Louis-Noel Pouchet
## Contact: <louis-noel.pouchet@inria.fr>
## 
## Started on  Mon May 25 17:26:48 2009 Louis-Noel Pouchet
## Last update Mon May 25 21:26:24 2009 Louis-Noel Pouchet
##

execute_command()
{
    echo "$1";
    $1;
    echo "Press ENTER to continue";
    read;
}

execute_command "pocc -c gemver.c"
execute_command "cat gemver.pocc.c"
execute_command "pocc -c gemver.c --codegen-timercode"
execute_command "./gemver.pocc"
execute_command "pocc -c gemver.c --pluto-tile --pluto-parallel --codegen-timercode"
execute_command "cat gemver.pocc.c"
execute_command "./gemver.pocc"
execute_command "pocc --verbose --letsee --pluto-tile --pluto-parallel --codegen-timercode gemver.c"
execute_command "cat iterative.dat"