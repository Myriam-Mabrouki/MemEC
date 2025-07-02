#! /bin/bash

# This script create a directory, compile TACLeBench programs 
# and move them into the created directory.

wget https://github.com/tacle/tacle-bench/archive/master.tar.gz
tar -xvzf master.tar.gz
mv master TACLeBench
rm master.tar.gz
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
