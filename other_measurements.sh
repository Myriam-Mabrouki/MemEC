#! /bin/bash

# Author: Myriam Mabrouki
# This script executes n times TACLeBench programs and retrieves the following information :
# total CPU time, number of cycles, number of instructions, total elapsed time.

# 1 - Check the parameters
if [ "$#" -ne 5 ]; then
    # Display the error message in stderr
    echo "Illegal number of parameters" >&2
    # Display the expected format in stderr
    echo "Expected : ./other_measurements.sh number_of_executions CPU_frequency_in_MHz memory_frequency_in_MHz measure_name measure_for_perf_cmd" >&2
    # End the program with an error
    exit 2
fi

# Parameters
N=$1			#1st argument = number of executions
fCPU=$2			#2nd argument = current CPU frequency
fMEM=$3			#3rd argument = current memory frequency
MEASURE_NAME=$4	#4th argument = name of the measure (for instance: exec time, number of instructions)
MEASURE=$5		#5th argument = keyword to get the corresponding measure(s) with the perf command
#MEASURE=-e task-clock  -e cycles -e instructions -e elapsed

# 2 - Execution N times
# For each task...
for PRGM in executables/*
do
	# For each execution
	for  ((i=0; i < $N; i++))
	do
		# Create "results" and "time_measures" directories if not exists and a directory for the task
		mkdir -p results/other_measures/${PRGM/*\/}/
		# Program executed in isolation in core 3
		# Total CPU time, number of cycles, number of instructions and total elapsed time retrieved
		taskset -c 3 perf stat $PRGM  2>&1 | grep $MEASURE | awk {'print $1'} >> "results/other_measures/${MEASURE_NAME}/${PRGM/*\/}/${PRGM/*\/}_${MEASURE_NAME}_fCPU_${fCPU}_fMEM_${fMEM}.txt"
	done
done
