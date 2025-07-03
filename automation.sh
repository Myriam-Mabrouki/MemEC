#! /bin/bash

# 1 - Check the parameters
if [ "$#" -ne 7 ]; then
    # Display the error message in stderr
    echo "Illegal number of parameters" >&2
    # Display the expected format in stderr
    echo "Expected : ./nom.sh number_of_executions program_name minimum_CPU_frequency_in_MHz maximum_CPU_frequency_in_MHZ step memory_frequency_in_MHz 1_or_0" >&2
    # End the program with an error
    exit 2
fi

# Parameters
N=$1		#1st argument = number of executions
PRGM=$2		#2nd argument = name of the chosen program
MIN=$3      #3rd argument = minimum frequency tested
MAX=$4      #4th argument = maximum frequency tested
STEP=$5     #5th argument = step
fMEM=$6
NRG=$7      #7th argument = 1 if we want to do energy consumption measures, 0 otherwise


# 2 - Initial configurations
# Set the CPUFreq governor
sudo cpupower frequency-set --governor userspace
# Set minimum and maximum frequencies
sudo cpupower frequency-set --min $MIN --max $MAX

# 3 - Measurements
# Choose between doing time or energy consumption measures
if [$NRG -eq 1]
then
    # Energy consumption measures
    for ((fCPU=$MIN; fCPU<=$MAX; fCPU=fCPU+$STEP))
    do
        # Set the frequency
        sudo cpupower frequency-set --freq $fCPU
        # Do the measures
        ./energy_measurements.sh $N $PRGM

    done
else
    # Time measures
    for ((fCPU=$MIN; fCPU<=$MAX; fCPU=fCPU+$STEP))
    do
        # Set the frequency
        sudo cpupower frequency-set --freq $fCPU
        # Do the measures
        ./time_measurements.sh $N $fCPU $fMEM

    done
fi