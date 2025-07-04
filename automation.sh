#! /bin/bash

# 1 - Check the parameters
if [ "$#" -ne 7 ] && [ "$#" -ne 0 ]; then
    # Display the error message in stderr
    echo "Illegal number of parameters" >&2
    # Display the expected format in stderr
    echo "Expected format is either :" >&2
    # First format
    echo "./nom.sh"
    echo "OR"
    # Second format
    echo "./nom.sh number_of_executions program_name minimum_CPU_frequency maximum_CPU_frequency step memory_frequency 1_or_0" >&2
    # Precisions about the first format
    echo "For the first expected format :" >&2
    echo " - parameters will be asked at runtime" >&2
    # Precisions about the second format
    echo "For the second format : " >&2
    echo "- frequencies are in MHz and the last parameter " >&2
    echo "- the last parameter is 1 for energy consumption measurements and 0 for time measurements"
    echo "- program_name could be set at 0 if you are doing time measuments since it will measure all sequential TACLeBench programs"
    # End the program with an error
    exit 2
fi

# Set the parameters
if [ "$#" -eq 0 ]
# During runtime
then
    # Ask user about the type of measurements
    echo "Enter 1 if you would like to carry on energy consumption measurements, 0 for time measurements:"
    # Read user's response
    read ENERGY_OR_TIME
    # Ask user about the number of executions
    echo "How many executions would you like to do?"
    read N
    # Ask user about frequencies
    echo "What is the minimum CPU frequency (in MHz) you want to use to carry on your experiments?"
    read MIN
    echo "What is the maximum CPU frequency (in MHz)?"
    read MAX
    echo "What step would you like to use between these two frequencies?"
    read STEP
    if (($ENERGY_OR_TIME != 1))
    then
        # Ask user about the program to measure for energy consumption
        echo "What program would you like to measure ?"
        read PRGM
    else
        # Ask user about memory frequency (which has to be already set)
        echo "What is the memory frequency you are currently using for your time measurements ?"
        read fMEM
    fi
# Before runtime
else
    # Parameters
    N=$1		#1st argument = number of executions
    PRGM=$2		#2nd argument = name of the chosen program
    MIN=$3      #3rd argument = minimum CPU frequency tested (in MHz)
    MAX=$4      #4th argument = maximum CPU frequency tested (in MHz)
    STEP=$5     #5th argument = step
    fMEM=$6     #5th argument = memory frequency in MHz
    NRG=$7      #7th argument = 1 if we want to do energy consumption measures, 0 otherwise
fi

# 2 - Initial configurations
# Set the CPUFreq governor
sudo cpupower frequency-set --governor userspace
# Set minimum and maximum frequencies
sudo cpupower frequency-set --min $(($MIN*1000)) --max $(($MAX*1000))

# 3 - Measurements
# Choose between doing time or energy consumption measures
if [ $ENERGY_OR_TIME -ne 1 ]
then
    # Energy consumption measures
    for ((fCPU=$MIN; fCPU<=$MAX; fCPU=fCPU+$STEP))
    do
        # Set the frequency
        sudo cpupower frequency-set --freq $(($fCPU*1000))
        # Do the measures
        # Space the measures
        sleep(1);
        ./energy_measurements.sh $N $PRGM
        # Space the measures
        sleep(1);
    done
else
    # Time measures
    for ((fCPU=$MIN; fCPU<=$MAX; fCPU=fCPU+$STEP))
    do
        # Set the frequency
        sudo cpupower frequency-set --freq $(($fCPU*1000))
        # Space the measures
        sleep(1);
        # Do the measures
        ./time_measurements.sh $N $fCPU $fMEM
        # Space the measures
        sleep(1);
    done
fi