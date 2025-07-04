#! /bin/bash

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
N=500			    # Number of executions
MIN=600                     # Minimum CPU frequency tested (in MHz)
MAX=1400                    # Maximum CPU frequency tested (in MHz)
STEP=400                    # Step
fMEM=500                    # Memory frequency in MHz
PRGM=executables/statemate  # Name of the chosen program
ENERGY_OR_TIME=1            # 0 for time measurements, 1 for energy measurements

# 3 - Initial configurations
# Set the CPUFreq governor
sudo cpupower frequency-set --governor userspace
# Set minimum and maximum frequencies
sudo cpupower frequency-set --min $(($MIN*1000)) --max $(($MAX*1000))

# 4 - Measurements
# Choose between doing time or energy consumption measures
if [ $ENERGY_OR_TIME -ne 0 ]
then
    # Energy consumption measures
    for ((fCPU=$MIN; fCPU<=$MAX; fCPU=fCPU+$STEP))
    do
        # Set the frequency
        sudo cpupower frequency-set --freq $(($fCPU*1000))
        # Do the measures
        # Space the measures
        sleep 1
        ./energy_measurements.sh $N $PRGM
        # Space the measures
        sleep 1
    done
else
    # Time measures
    for ((fCPU=$MIN; fCPU<=$MAX; fCPU=fCPU+$STEP))
    do
        # Set the frequency
        sudo cpupower frequency-set --freq $(($fCPU*1000))
        # Space the measures
        sleep 1
        # Do the measures
        ./time_measurements.sh $N $fCPU $fMEM
        # Space the measures
        sleep 1
    done
fi
