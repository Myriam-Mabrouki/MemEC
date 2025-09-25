#! /bin/bash

# Author: Myriam Mabrouki
# This script 

mkdir -p results/energy_results/
for dir in results/energy_measures/*
do
    cd $dir
    res_file=$(echo -n $dir | cut -d "/" -f3).csv
    echo \"Value\",\"CPU freq\",\"Memory freq\" >> ../../energy_results/$res_file
    for file in *
    do
        fCPU=$(echo -n $file | cut -d "_" -f3)
        fMEM=$(echo -n $file | cut -d "_" -f5)
        isPreemptRt=$(echo -n $file | cut -d "_" -f6)
        energy=$(tail -n 1 $file| cut -d "," -f2) #| awk -F "," {'print $2'} works as well
        echo $energy $fCPU $fMEM >> ../../energy_results/$res_file
    done
    cd ../../../
done