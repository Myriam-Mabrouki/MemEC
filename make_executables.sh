#! /bin/bash

# This script create a directory, compile TACLeBench programs
# and move them into the created directory.

# 1 - Get TACLeBench tasks
# Download TACLeBench repository from github
wget https://github.com/tacle/tacle-bench/archive/master.tar.gz
# Exact the .tar.gz file
tar -xvzf master.tar.gz
# Delete the .tar.gz files (.1 if it already existed)
rm -f master.tar.gz master.tar.gz.1

# 2 - Create a directory
# Delete "executables" directory if it exists
rm -rf executables
# Create a new one
mkdir executables
# Go to the location of the programs
cd tacle-bench-master/bench/sequential/

# 3 - Compile and move each task into "executables" directory
# For each task...
for DIR in *
do
	# Display directory's name (which is also the task's name)
	echo $DIR
	# Go to its directory
	cd $DIR
	# Compile it
	gcc *.c
	# Rename the executable
	mv a.out $DIR
	# Move the executable into the dedicated directory
	mv $DIR ../../../../executables
	cd ..
done
