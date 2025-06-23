# MemEC
This repository contains **memory energy consumption experiments**.

## Programs 

- ```make_executables.sh``` : compile executables of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) 
- ```time_measurements.sh```: execute programs of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) in isolation N times with the highest user priority and retrieves data about them (total execution time, total CPU times, number of cycles and number of instructions)
- ```cpu_isolation.sh``` : isolate one CPU with cgroups
- ```energy_executions.sh``` : execute one program N times with the highest user priority
- ```memory_stress.c``` : program that stresses the memory (with malloc)


## Experiments 

- **Step one :** Grant execute permissions for shell programs with the following command :
  ```chmod +x make_executables.sh time_measurements.sh energy_executions.sh energy_executions.sh cpu_isolation.sh```
- **Step two :** Download the repository of of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) and unzip it.
- **Step three :** Execute ```./make_executables.sh```. Now you have a folder untitled _executables_ with the sequential TACLeBench programs.
- **Step four :** 
