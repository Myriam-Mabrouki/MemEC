#! /bin/bash

# This script executes n times a chosen program
# This script is used in our experiments for executing a program 
# while measuring the whole energy consumpion.

n=$1		#1st argument = number of executions
prgm=$2		#2nd argument = name of the chosen program

# The chosen program has its priority set to the maximum possible
# (in the user space)
for  ((i=0; i<$n; i++))
do
	sudo nice -n -20 ./$prgm
done
