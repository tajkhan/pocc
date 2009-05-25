#!/bin/sh
## demo-dgemm.sh for gemver in /Users/pouchet
## 
## Made by Louis-Noel Pouchet
## Contact: <louis-noel.pouchet@inria.fr>
## 
## Started on  Mon May 25 17:26:48 2009 Louis-Noel Pouchet
## Last update Mon May 25 21:26:16 2009 Louis-Noel Pouchet
##

execute_command()
{
    echo "$1";
    $1;
    echo "Press ENTER to continue";
    read;
}

execute_command "pocc -c dgemm.c"
execute_command "cat dgemm.pocc.c"
execute_command "pocc -c dgemm.c --codegen-timercode"
execute_command "./dgemm.pocc"
execute_command "pocc -c dgemm.c --pluto-tile --codegen-timercode"
execute_command "cat dgemm.pocc.c"
execute_command "./dgemm.pocc"
execute_command "pocc --verbose --letsee --letsee-walk random --letsee-space schedule --letsee-rtries 15 --codegen-timercode dgemm.c"
execute_command "cat iterative.dat"