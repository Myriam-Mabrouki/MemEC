# MemEC
This repository contains **memory energy consumption experiments**.

## Programs 

- ```make_executables.sh``` : compile executables of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) 
- ```time_measurements.sh```: execute programs of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) in isolation N times with the highest user priority and retrieves data about them (total execution time, total CPU times, number of cycles and number of instructions)
- ```cpu_isolation.sh``` : isolate one CPU with cgroups
- ```energy_executions.sh``` : execute one program N times with the highest user priority
- ```memory_stress.c``` : program that stresses the memory (with malloc)


## Experiment

### Hardware and software configurations

For this experiment, we measure the energy consumption of a [Raspberry Pi 3B+](https://www.raspberrypi.com/products/raspberry-pi-3-model-b-plus/) with the power analyser [Otii Ace Pro](https://www.qoitech.com/otii-ace/).
The operating system used in the Raspberry is a Raspberry Pi OS, an operating system based [Debian GNU/Linux](https://www.debian.org/) version 12 (bookworm).

### Protocol

- **Step one :** Grant execute permissions for shell programs with the following command :
  ```chmod +x make_executables.sh time_measurements.sh energy_executions.sh energy_executions.sh cpu_isolation.sh```
- **Step two :** Download the repository of of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) and unzip it.
- **Step three :** Execute ```./make_executables.sh```. Now you have a folder untitled _executables_ with the sequential TACLeBench programs.
- **Step four :** 
- **Step five :**
- **Step six :**
- **Step seven :**
- **Step eight :**
- **Step nine :**
- **Step ten :**
