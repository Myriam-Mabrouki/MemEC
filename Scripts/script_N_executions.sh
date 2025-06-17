#! /bin/bash

# This script executes N times a chosen program

N=$1		#1st argument = number of executions
prgm=$2		#2nd argument = name of the chosen program

for  ((i=0; i < $N; i++))
do
	./$prgm
done