# MemEC
This repository contains **memory energy consumption experiments**.

## Table of contents

1. [Files includes](#1-files-included)
    1. [```make_executables.sh```](#11-make_executablessh)
    2. [```time_measurements.sh```](#12-time_measurementssh)
    3. [```energy_executions.sh```](#13-energy_executionssh)
    4. [```automation_measures.sh```](#14-automation_measuressh)
    5. [``csv_update.c```](#15-csv_updatec)
    6. [```time_stats.c```](#16-time_statsc)
2. [Preliminaries](#2-preliminaries)
    1. [Internet connection](#21-internet-connection)
    2. [Required tools](#22-required-tools)
    3. [Required configuration](#23-required-configuration)
3. [Experiments](#3-experiments)
    1. [Hardware and software configurations](#31-hardware-and-software-configurations)
    2. [Protocol](#32-experimental-protocol)
        1. [General protocol](#321-general-protocol)
        2. [Time measurements](#322-time-measurements)
        3. [Energy measurements](#323-energy-measurements)


## 1. Files included 

- ```make_executables.sh``` : download and compile executables of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) 
- ```time_measurements.sh```: execute programs of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) in isolation $N$ times with the highest user priority and retrieves data about them (total execution time, total CPU times, number of cycles and number of instructions)
- ```energy_executions.sh``` : execute one program $N$ times with the highest user priority
- ```automation_measures.sh``` : automatise time and energy measures
- ```csv_update.c``` : updates in CSV files obtained after energy measures
- ```time_stats.c``` : statistics about programs of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2)

### 1.1. ```make_executables.sh```

### 1.2. ```time_measurements.sh```

### 1.3. ```energy_executions.sh```

### 1.4. ```automation_measures.sh```

### 1.5. ```csv_update.c```

### 1.6. ```time_stats.c```

## 2. Preliminaries

### 2.1. Internet connection

Make sure you are connected to Internet so the ```make_executables.sh``` program could work.
If you cannot have an Internet connection on your platform, comment the whole **"1 - Get TACLeBench tasks"** part in ```make_executables.sh```, download [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) repository by yourself and transfer it to your platform.

### 2.2. Required tools

Before starting the experiments, make you sure you have these tools install :
- perf
- cpupower

If you do not have these tools installed and you have the same software configurations ([Raspberry Pi OS](https://www.raspberrypi.com/software/) or [Debian GNU/Linux](https://www.debian.org/)), you can follow next steps : 
```
sudo apt install linux-perf
sudo apt install linux-cpupower -y
```
Otherwise, you can look for the installation process corresponding to the OS you are using.

### 2.3. Required configuration

To perform measures in isolation, we set the parameter ```isolcpus``` in the  ```/boot/firmware/cmdline.txt``` (or ```/boot/cmdline.txt```) file.
This feature allows you to isolate specific cores so that no process run on them unless you assign a process to an isolated core.

The following steps show how to isolate all the cores except the core 0 : 
- find the file to edit : 
```
sudo nano /boot/firmware/cmdline.txt
``` 
or 
```
sudo nano /boot/cmdline.txt
``` 
- edit the file by adding at the end of the line : ``` isolcpus=1-3```
- restart your device to take effect : 
```
sudo reboot
```


## 3. Experiments

### 3.1. Hardware and software configurations

For this experiment, we measure the energy consumption of a [Raspberry Pi 3B+](https://www.raspberrypi.com/products/raspberry-pi-3-model-b-plus/) with the power analyser [Otii Ace Pro](https://www.qoitech.com/otii-ace/).
The [Raspberry Pi 3B+](https://www.raspberrypi.com/products/raspberry-pi-3-model-b-plus/) has a quad-core processor and a 1GB LPDDR2 SDRAM.
The operating system used on the system on chip is a [Raspberry Pi OS](https://www.raspberrypi.com/software/), an operating system based [Debian GNU/Linux](https://www.debian.org/) version 12 (bookworm).
The measures are performed on a single core (check [section 2.3.](#23-required-configuration)). 

### 3.2. Experimental protocol

#### 3.2.1 General protocol

- **Step one :** Grant execute permissions for shell programs with the following command :
```
chmod +x *.sh
```
- **Step two :** Execute the script ```make_executables.sh```.
```
./make_executables.sh
```
Now you have a folder untitled _executables_ with the sequential [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) programs.

⚠️ <ins>Caution</ins> : This program will not work if you are not connected to Internet. If so, please check [section 2.1.](#21-internet-connection)

- **Step three :** Set the memory frequency in ```/boot/firmware/cmdline.txt``` by using the command :
```
sudo nano /boot/firmware/cmdline.txt
```
Then you will have to add the following lines : 
```
# Set frequencies
sdram_freq=X
sdram_freq_min=X
```
and replace $X$ by the memory frequency you want.
- **Step four :** Reboot your device so the memory frequency change could take effect.
```
sudo reboot
```
- **Step five :** Set the parameters in ```automation_measures.sh```. You have to modify the file directly in order to avoid useless I/O. The parameters are the following :

| Parameter       | Description                                                                     | Default value           |
| --------------- | ------------------------------------------------------------------------------- | ----------------------- |
| $\text{N}$      | number of executions                                                            | $500$                   |
| $\text{MIN}$    | minimum CPU frequency tested (in MHz)                                           | $600$                   |
| $\text{MAX}$    | maximum CPU frequency tested (in MHz)                                           | $1400$                  |
| $\text{STEP}$   | the step between two CPU frequencies tested                                     | $400$                   |
| $\text{fMEM}$   | memory frequency set (in MHz)                                                   | $500$                   |
| $\text{PRGM}$   | name of the chosen program (in case of energy measures)                         | executables/statemate   |
| $\text{ENERGY}$ | boolean (0 for time measurements, 1 for energy measurements)                    | $1$                     |

- **Step six :** Launch the program ```automation_measures.sh``` with : 
```
sudo ./automation_measures.sh
```
- **Step seven :** If you want to test with other memory frequencies, go back to **Step three**. In our case, we test the following memory frequencies : $500$ MHz, $400$ MHz, $300$ MHz, $200$ MHz.

#### 3.2.2 Time measurements

#### 3.2.3 Energy measurements
