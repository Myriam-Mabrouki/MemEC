# MemEC
This repository contains **memory energy consumption experiments**

## Programs 

- ```make_executables.sh``` : compile executables of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) 
- ```time_measurements.sh```: execute programs of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) in isolation N times with the highest user priority and retrieves data about them (total execution time, total CPU times, number of cycles and number of instructions)
- ```energy_executions.sh``` : execute one program N times with the highest user priority
- ```cpu_isolation.sh``` : isolate one CPU with cgroups
- ```memory_stress.c``` : program that stresses the memory (with malloc)
