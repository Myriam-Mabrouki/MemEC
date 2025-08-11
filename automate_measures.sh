#! /bin/bash

# Author: Myriam Mabrouki
# This script executes either energy_measurements.sh or time_measurements.sh and changes CPU frequency between two consecutive executions. 

# 1 - Check the parameters
if [ "$#" -ne 0 ]
then
	    # Display the error message in stderr
	    echo "Illegal number of parameters" >&2
	    # Display the expected format in stderr
	    echo "Expected format is : ./automation_measures" >&2
	    exit 2
fi

# 2 - Set the parameters
# Parameters are set in the file to avoid I/O
N=1000			            # Number of executions
MIN=600                     # Minimum CPU frequency tested (in MHz)
MAX=1400                    # Maximum CPU frequency tested (in MHz)
STEP=100                    # Step between two CPU frequencies tested
fMEM=200                    # Memory frequency in MHz
PRGM=executables/statemate  # Name of the chosen program
ENERGY=1                    # 0 for energy measurements, other measurements otherwise
MEASURE_NAME=execution_time	# name of the measure in case of other measurements
MEASURE=elapsed				# keyword for the other measure to use with the perf command

# 3 - Initial configurations
# Set the CPUFreq governor
cpupower frequency-set --governor userspace
# Set minimum and maximum frequencies
cpupower frequency-set --min $(($MIN*1000)) --max $(($MAX*1000))

# 4 - Measurements
# Choose between doing time or energy consumption measures
if [ $ENERGY -eq 0 ]
then
    	# Energy consumption measures
    	for ((fCPU=$MIN; fCPU<=$MAX; fCPU=fCPU+$STEP))
    	do
        	# Set the frequency
	        cpupower frequency-set --freq $(($fCPU*1000))
        	# Space the measures
	        sleep 1
        	# Do the measures
	       ./energy_measurements.sh $N $PRGM
        	# Space the measures
	        sleep 1
    	done
else
	    # Time measures
	    for ((fCPU=$MIN; fCPU<=$MAX; fCPU=fCPU+$STEP))
	    do
	        # Set the frequency
	        cpupower frequency-set --freq $(($fCPU*1000))
        	# Space the measures
	        sleep 1
	        # Do the measures
        	./other_measurements.sh $N $fCPU $fMEM $MEASURE_NAME $MEASURE
	        # Space the measures
        	sleep 1
	    done
fi