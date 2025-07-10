#! /bin/bash

# This script executes n times a chosen program
# This script is used in our experiments for executing a program
# while measuring the whole energy consumpion.

# 1 - Check the parameters
if [ "$#" -ne 2 ]; then
    # Display the error message in stderr
    echo "Illegal number of parameters" >&2
    # Display the expected format in stderr
    echo "Expected : ./energy_measurements.sh number_of_executions program_name" >&2
    # End the program with an error
    exit 2
fi

# Parameters
N=$1		#1st argument = number of executions
PRGM=$2		#2nd argument = name of the chosen program

# 2 - Execution N times
# For each execution...
for  ((i=0; i<$N; i++))
do
    # Program executed in isolation in core 3 with its priority set to the maximum
	taskset -c 3 chrt 99 ./$PRGM
done
