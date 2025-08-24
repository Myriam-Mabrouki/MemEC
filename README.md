# MemEC
This project is about **memory energy consumption experiments** intended for my final-year internship at INRIA Paris.
The internship is entitled: "Introduction of statistical models for energy in temporal analyses".

## Table of contents

- [MemEC](#memec)
  - [Table of contents](#table-of-contents)
  - [1. Files included](#1-files-included)
    - [1.1. Quick presentation of all files](#11-quick-presentation-of-all-files)
    - [1.2. ```make_executables.sh```](#12-make_executablessh)
    - [1.3. ```other_measurements.sh```](#13-other_measurementssh)
    - [1.4. ```energy_executions.sh```](#14-energy_executionssh)
    - [1.5. ```automate_measures.sh```](#15-automate_measuressh)
    - [1.6. ```csv_update.c```](#16-csv_updatec)
    - [1.7. ```time_stats.c```](#17-time_statsc)
  - [2. Preliminaries](#2-preliminaries)
    - [2.1. Internet connection](#21-internet-connection)
    - [2.2. Required tools](#22-required-tools)
    - [2.3. Required configuration](#23-required-configuration)
      - [2.3.1 CPU isolation](#231-cpu-isolation)
      - [2.3.2. Real-time system](#232-real-time-system)
  - [3. Experiments](#3-experiments)
    - [3.1. Hardware and software configurations](#31-hardware-and-software-configurations)
    - [3.2. Experimental protocol](#32-experimental-protocol)
      - [3.2.1 General protocol](#321-general-protocol)
      - [3.2.2 Time measurements](#322-time-measurements)
      - [3.2.3 Energy measurements](#323-energy-measurements)


## 1. Files included 

### 1.1. Quick presentation of all files

- ```make_executables.sh``` : download and compile executables of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) 
- ```other_measurements.sh```: execute programs of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) in isolation $N$ times with the highest user priority and retrieves requested data about them (total execution time, total CPU times, number of cycles, number of instructions...)
- ```energy_executions.sh``` : execute one program $N$ times with the highest user priority
- ```automate_measures.sh``` : automate energy and other measures
- ```csv_update.c``` : updates in CSV files obtained after energy measures
- ```time_stats.c``` : statistics about programs of [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2)
- results: directory containing data files obtained
- results/energy_measures: directory with CSV files containing power consumption
- results/other_measures: directory containing a directory per (here from [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2)) program containing data other than energy consumption

### 1.2. ```make_executables.sh```

### 1.3. ```other_measurements.sh```

### 1.4. ```energy_executions.sh```

### 1.5. ```automate_measures.sh```

### 1.6. ```csv_update.c```

### 1.7. ```time_stats.c```

## 2. Preliminaries

### 2.1. Internet connection

Make sure you are connected to Internet so the ```make_executables.sh``` program could work.
If you cannot have an Internet connection on your platform, comment the whole **"1 - Get TACLeBench tasks"** part in ```make_executables.sh```, download [TACLeBench](http://dx.doi.org/10.4230/OASIcs.WCET.2016.2) repository by yourself and transfer it to your platform.

### 2.2. Required tools

Before starting the experiments, make you sure you have these tools install :
- perf
- cpupower

If you do not have these tools installed and you have the same software configurations ([Raspberry Pi OS](https://www.raspberrypi.com/software/) or [Debian GNU/Linux](https://www.debian.org/)), you can follow next steps : 
```shell
sudo apt install linux-perf -y
sudo apt install linux-cpupower -y
```
Otherwise, you can look for the installation process corresponding to the OS you are using.

### 2.3. Required configuration

#### 2.3.1 CPU isolation

To perform measures in isolation, we set the parameter ```isolcpus``` in the  ```/boot/firmware/cmdline.txt``` (or ```/boot/cmdline.txt```) file.
This feature allows you to isolate specific cores so that no process run on them unless you assign a process to an isolated core.

The following steps show how to isolate all the cores except the core 0 : 
- find the file to edit : 
```shell
sudo nano /boot/firmware/cmdline.txt
``` 
or 
```shell
sudo nano /boot/cmdline.txt
``` 
- edit the file by adding at the end of the line : ``` isolcpus=1-3```
- restart your device to take effect : 
```shell
sudo reboot
```

#### 2.3.2. Real-time system

To simulate a real-time system, we use the ```PREEMPT_RT``` patch.

```shell
./preempt_rt.sh
```

```shell
make menuconfig
```
Within the menu navigate to General Setup/Preemption Model and choose Fully Preemptible Kernel. 

 Alternately, make the following change to the Linux configuration (.config file ?):
```
-CONFIG_PREEMPT=y
+CONFIG_PREEMPT_RT_FULL=y
```

Finally, build the kernel.
```shell
make
```

You can now restart your device for changes to take effect.
```shell
sudo reboot
```

## 3. Experiments

### 3.1. Hardware and software configurations

For this experiment, we measure the energy consumption of a [Raspberry Pi 3B+](https://www.raspberrypi.com/products/raspberry-pi-3-model-b-plus/) with the power analyser [Otii Ace Pro](https://www.qoitech.com/otii-ace/).
The [Raspberry Pi 3B+](https://www.raspberrypi.com/products/raspberry-pi-3-model-b-plus/) has a quad-core processor and a 1GB LPDDR2 SDRAM.
The operating system used on the system on chip is a [Raspberry Pi OS](https://www.raspberrypi.com/software/), an operating system based [Debian GNU/Linux](https://www.debian.org/) version 12 (bookworm).
The measures are performed on a single core (check [section 2.3.1](#231-cpu-isolation)). 

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

> [!caution]
>
> This program will not work if you are not connected to Internet. If so, please check [section 2.1.](#21-internet-connection)

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
```shell
sudo reboot
```
- **Step five :** Set the parameters in ```automate_measures.sh```. You have to modify the file directly in order to avoid useless I/O. The parameters are the following :

| Parameter       | Description                                                                     | Default value           |
| --------------- | ------------------------------------------------------------------------------- | ----------------------- |
| $\text{N}$      | number of executions                                                            | $1000$                  |
| $\text{MIN}$    | minimum CPU frequency tested (in MHz)                                           | $600$                   |
| $\text{MAX}$    | maximum CPU frequency tested (in MHz)                                           | $1400$                  |
| $\text{STEP}$   | the step between two CPU frequencies tested                                     | $100$                   |
| $\text{fMEM}$   | memory frequency set (in MHz)                                                   | $500$                   |
| $\text{PRGM}$   | name of the chosen program (in case of energy measures)                         | executables/ndes        |
| $\text{ENERGY}$ | boolean (0 for energy measurements, anything else otherwise)                    | $1$                     |

- **Step six :** Launch the program ```automate_measures.sh``` with : 
```shell
sudo ./automation_measures.sh
```
- **Step seven :** If you want to test with other memory frequencies, go back to **Step three**. In our case, we test the following memory frequencies : $500$ MHz, $400$ MHz, $300$ MHz, $200$ MHz.

#### 3.2.2 Time measurements

#### 3.2.3 Energy measurements
