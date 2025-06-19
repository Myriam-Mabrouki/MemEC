#! /bin/bash

# This script executes n times TACLeBench programs and retrieves the following information : 
# total CPU time, number of cycles, number of instructions, total elapsed time.

n=$1		#1st argument = number of executions
fCPU=$2		#2nd argument = current CPU frequency
fMEM=$3		#3rd argument = current memory frequency

# Delete "executables" directory if it exists and create a new one
rm -rf ./time_measures_${fCPU}_${fMEM}
mkdir time_measures_${fCPU}_${fMEM}

for prgm in executables/*
do
	for  ((i=0; i < $n; i++))
	do
		# The program has its priority set to the maximum possible
		# (in the user space)
		sudo perf stat nice -n -20 $prgm  2>&1 | grep -e task-clock  -e cycles -e instructions -e elapsed  | awk {'print $1'} >> "time_measures_${fCPU}_${fMEM}/${prgm/*\/}_time_measure_fCPU_${fCPU}_fMEM_${fMEM}.txt"
	done
done
