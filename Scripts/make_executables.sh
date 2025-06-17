#! /bin/bash

# This script create a directory, compile TACLeBench programs 
# and move them into the created directory.

# Delete "executables" directory if it exists, create a new one
# and go to the location of the programs 
rmdir -rf executables
mkdir executables
cd tacle-bench-master/bench/sequential/

# For each program compile it and move the executables
# into the previously created directory
for dir in *
do	
	echo $dir
	cd $dir
	gcc *.c
	mv a.out $dir
	mv $dir ../../../../executables
	cd ..
done
