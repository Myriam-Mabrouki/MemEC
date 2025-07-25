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
STEP=400                    # Step between two CPU frequencies tested
fMEM=500                    # Memory frequency in MHz
PRGM=executables/statemate  # Name of the chosen program
ENERGY=1                    # 0 for time measurements, 1 for energy measurements

# 3 - Initial configurations
# Set the CPUFreq governor
cpupower frequency-set --governor userspace
# Set minimum and maximum frequencies
cpupower frequency-set --min $(($MIN*1000)) --max $(($MAX*1000))

# 4 - Measurements
# Choose between doing time or energy consumption measures
if [ $ENERGY -ne 0 ]
then
	# Check if the file containing execution time exists
	if ! [ -f results/time_measures/${PRGM/*\/}/${PRGM/*\/}_results.txt ]; then
  		echo "exec_time,CPU_freq,MEM_freq" >> "results/time_measures/${PRGM/*\/}/${PRGM/*\/}_results.txt"
	fi
    	# Energy consumption measures
    	for ((fCPU=$MIN; fCPU<=$MAX; fCPU=fCPU+$STEP))
    	do
        	# Set the frequency
	        cpupower frequency-set --freq $(($fCPU*1000))
        	# Space the measures
	        sleep 1
        	# Do the measures
	        DURATION=`perf stat ./energy_measurements.sh $N $PRGM 2>&1 | grep elapsed  | awk '{print $1}'`
        	# Space the measures
	        sleep 1
		# Write the duration
		echo $DURATION,$fCPU,$fMEM >> "results/time_measures/${PRGM/*\/}/${PRGM/*\/}_results.txt"
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
        	./time_measurements.sh $N $fCPU $fMEM
	        # Space the measures
        	sleep 1
	    done
fi
