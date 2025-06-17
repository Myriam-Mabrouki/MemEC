#! /bin/bash

# This script executes n times a chosen program

n=$1		#1st argument = number of executions
prgm=$2		#2nd argument = name of the chosen program

for  ((i=0; i < $n; i++))
do
	./$prgm
done
