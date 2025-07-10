#! /bin/bash

# This script executes n times TACLeBench programs and retrieves the following information :
# total CPU time, number of cycles, number of instructions, total elapsed time.

# 1 - Check the parameters
if [ "$#" -ne 3 ]; then
    # Display the error message in stderr
    echo "Illegal number of parameters" >&2
    # Display the expected format in stderr
    echo "Expected : ./time_measurements.sh number_of_executions CPU_frequency_in_MHz memory_frequency_in_MHz" >&2
    # End the program with an error
    exit 2
fi

# Parameters
N=$1		#1st argument = number of executions
fCPU=$2		#2nd argument = current CPU frequency
fMEM=$3		#3rd argument = current memory frequency

# 2 - Execution N times
# For each task...
for PRGM in executables/*
do
	# For each execution
	for  ((i=0; i < $N; i++))
	do
		# Create "results" and "time_measures" directories if not exists and a directory for the task
		mkdir -p results/time_measures/${PRGM/*\/}
		# Program executed in isolation in core 3
		# Total CPU time, number of cycles, number of instructions and total elapsed time retrieved
		taskset -c 3 perf stat $PRGM  2>&1 | grep -e task-clock  -e cycles -e instructions -e elapsed  | awk {'print $1'} >> "results/time_measures/${PRGM/*\/}/${PRGM/*\/}_time_measure_fCPU_${fCPU}_fMEM_${fMEM}.txt"
	done
done
