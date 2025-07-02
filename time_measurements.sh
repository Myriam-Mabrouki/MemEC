#! /bin/bash

# This script executes n times TACLeBench programs and retrieves the following information :
# total CPU time, number of cycles, number of instructions, total elapsed time.

# 1 - Check the parameters
if [ "$#" -ne 3 ]; then
    # Display the error message in stderr
    echo "Illegal number of parameters" >&2
    # Display the expected format in stderr
    echo "Expected : ./time_measurements.sh number_of_executions CPU_frequency memory_frequency" >&2
    # End the program with an error
    exit 2
fi

# Parameters
n=$1		#1st argument = number of executions
fCPU=$2		#2nd argument = current CPU frequency
fMEM=$3		#3rd argument = current memory frequency

# 2 - Create a directory
# Delete "executables" directory if it exists
rm -rf ./time_measures_${fCPU}_${fMEM}
# Create a new one
mkdir time_measures_${fCPU}_${fMEM}

# 3 - Execution time measures
# For each task...
for prgm in executables/*
do
	# For each execution
	for  ((i=0; i < $n; i++))
	do
		# Program executed in isolation in core 3
		# Total CPU time, number of cycles, number of instructions and total elapsed time retrieved
		taskset -c 3 sudo perf stat $prgm  2>&1 | grep -e task-clock  -e cycles -e instructions -e elapsed  | awk {'print $1'} >> "time_measures_${fCPU}_${fMEM}/${prgm/*\/}_time_measure_fCPU_${fCPU}_fMEM_${fMEM}.txt"
	done
done
