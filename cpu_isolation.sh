#! /bin/bash

# This script is used to isolate one CPU on a quad-core processor with one memory (RPI 3B+).
# This script is mainly inspired by the documentation in linux.enea


# 1 - Configure the CPU sets

# a. Create the cpusets (RT & nRT)

# enable the creation of cpuset folder
sudo mount -t tmpfs none /sys/fs/cgroup

# create the cpuset folder and mount the cgroup filesystem
sudo mkdir /sys/fs/cgroup/cpuset
sudo mount -t cgroup -o cpuset none /sys/fs/cgroup/cpuset

# create the partition
sudo mkdir /sys/fs/cgroup/cpuset/rt
sudo mkdir /sys/fs/cgroup/cpuset/nrt

# b. Add the general purpose CPUs to the nRT set
echo 0-2 | sudo tee /sys/fs/cgroup/cpuset/nrt/cpuset.cpus

# c. Add the real-time CPU to the RT set
echo 3 | sudo tee /sys/fs/cgroup/cpuset/rt/cpuset.cpus

# d. Make the CPU in the RT set exclusive, i.e. do not let tasksin other sets use them
echo 1 | sudo tee /sys/fs/cgroup/cpuset/rt/cpuset.cpu_exclusive


# 2 - Configure memory

# a. Associate the nRT set with memory node 0
echo 0 | sudo tee /sys/fs/cgroup/cpuset/nrt/cpuset.mems

# b. Associate the RT set with memory node 0
echo 0 | sudo tee /sys/fs/cgroup/cpuset/rt/cpuset.mems


# 3 - Configure load balancing

# a. Disable load balancing in the root cpuset, this is necessary for settings in the child cpusets to take effect
echo 0 | sudo tee /sys/fs/cgroup/cpuset/cpuset.sched_load_balance

# b. Disable load balancing in the RT cpuset
echo 0 | sudo tee /sys/fs/cgroup/cpuset/rt/cpuset.sched_load_balance

# b. Enable load balancing in the RT cpuset
echo 1 | sudo tee /sys/fs/cgroup/cpuset/nrt/cpuset.sched_load_balance


# 4 - Move general purpose tasks to the GP partition

# Define the input file
INFILE=processes

# Write PIDs of all running tasks in the input file
ps aux | awk {'print $2'} > processes

# Read the input file line by line
while read -r LINE
do
    # Move general purpose tasks to the GP partition
    echo $LINE | sudo tee /sys/fs/cgroup/cpuset/nrt/tasks
done < "$INFILE"


# 5 - Move IRQs  to the GP CPUs
