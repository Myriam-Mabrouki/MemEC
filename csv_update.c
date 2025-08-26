/* Author : Myriam Mabrouki
* This file is used to update CSV files in the context of energy consumption experiments.
* (More details in : https://github.com/Myriam-Mabrouki/MemEC)
* Each row, except the header, in original CSV files has the format : x1,x2
* where x1 is a timestamp value and x2 is a power consumption value.
* 1. The first update adjusts the time values by starting the first one at 0.
* 2. The second update adds CPU and memory frequencies to each row of CVS files.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#define MAX_LENGTH 1024
#define NB_EXEC 1000


/* This function updates the CSV by making the first timestamp start at 0 
* and adjusting other time values accordingly.
* This function considers there is a header in the CSV file.
* FILE *input_file: CSV input file
* FILE *output_file: CSV output file
* int __1, int __2: parameters only used to match the "operation" function 
* that takes a function pointer as an input
* Returns 0 if there is no error.
*/
int begin_at_0( FILE *input_file, 
				FILE *output_file, 
				int __1, 
				int __2)
{
	// Line counter
	int lc = 0;
	// Buffer to store each line of the input file.
	char line[MAX_LENGTH];
	// Buffer to store the new line going to the output file.
	char newline[MAX_LENGTH];
	// Variable to store timestamp and power values.
	float timestamp, first_timestamp, value;

	// Read each line from the input file and store it in the 'line' buffer.
	while (fgets(line, sizeof(line), input_file)) {

		// Retrieve time and power values.
		char tmp[MAX_LENGTH];
        strcpy(tmp, line);
        timestamp = atof(strtok(tmp, ","));
		value = atof(strtok(NULL, ","));

		// Special case for the first line.
		if (lc == 0) {
			fputs(line, output_file);
			lc++;
			continue;
		}
		// Special case for the second line.
		else if (lc == 1) {
			first_timestamp = timestamp;
			lc++;
		}

		// Modify the time value value and write the power value.
		sprintf(newline, "%f,%f\n", timestamp - first_timestamp, value);
		// Write the modified line into the output file.
		fputs(newline, output_file);
	}

	return 0;
}


/* This function updates the CSV by adding CPU and memory frequencies to each row.
* This function considers:
* - each file has a single CPU frequency and a single memory frequency
* - each file has a header.
* (Thus, in one file, all rows have the same CPU frequency and the same memory frequency).
* FILE *input_file: CSV input file
* FILE *output_file: CSV output file
* int CPU_freq: CPU frequency
* int MEM_freq: Memory frequency
* Returns 0 if there is no error.
*/
int add_CPU_freq_MEM_freq_in_csv(	FILE *input_file, 
									FILE *output_file, 
									int CPU_freq, 
									int MEM_freq)
{
	// First line boolean
	char first = 0;
	// Buffer to store each line of the input file.
	char line[MAX_LENGTH];
	// Buffer to store the additional part going to the output file.
	char addition[MAX_LENGTH];

	// Read each line from the input file and store it in the 'line' buffer.
	while (fgets(line, sizeof(line), input_file)) {
		// Create the additional part
		if (!first) {
			// Special case for the header
			sprintf(addition, ",\"CPU freq\",\"Memory freq\"\n");
			first++;
		}
		else {
			// Adding CPU and memory frequencies otherwise
			sprintf(addition, ",%d,%d\n", CPU_freq, MEM_freq);
		}
		// Remove the newline character.
		line[strcspn(line, "\n")] = 0;
		// Concatenate the additional part to the input line.
		strcat(line, addition);
		// Write the modified line into the output file.
		fputs(line, output_file);
	}
	
	return 0;
}

/* This function opens a file, applies a function to it, 
* creates an updated file and replaces the original file by the updated file.
* char *input_filename: name of the input file
* int CPU_freq: CPU frequency
* int MEM_freq: Memory frequency
* int (*fct)(FILE*, FILE*, int, int): function applied to the input file.
* Returns 0 if there is no error.
*/
int operation(	char *input_filename, 
				int CPU_freq, 
				int MEM_freq, 
				int (*fct)(FILE*, FILE*, int, int))
{
	// Open files
	FILE* input_file = fopen(input_filename, "r");
	FILE* output_file = fopen("tmp", "w");
	
	// Handle opening errors
	if (input_file == NULL) {
		// Print an error message to the standard error stream if the input file cannot be opened.
		fprintf(stderr, "fopen errorr: Unable to open input file!\n");
		return EXIT_FAILURE;
	}
	if (output_file == NULL) {
		// Print an error message to the standard error stream if the output file cannot be opened.
		fprintf(stderr, "fopen errorr: Unable to open output file!\n");
		return EXIT_FAILURE;
	}
	
	// Main operation
	fct(input_file, output_file, CPU_freq, MEM_freq);
	
	// Close the file streams.
	fclose(input_file);
	fclose(output_file);

	// Replace the original file with the temporary file
	if (remove(input_filename) != 0) {
		fprintf(stderr, "remove error: Could not remove original file\n");
		remove("tmp");
		return EXIT_FAILURE;
	}
	if (rename("tmp", input_filename) != 0) {
		fprintf(stderr, "rename error: Could not rename temporary file\n");
		return EXIT_FAILURE;
	}

	return 0;
}

/* This function retrieves CPU and memory frequencies of a CSV file.
* This function considers:
* - CPU and memory frequencies are in the filename
* - CPU frequency is prior to the memory frequency in the filename
* - CPU and memory frequencies are the only integers in the filename
* - each part in the filename is separated by the character : "_".
* char *filename: name of the input file (but not the whole path)
* int *CPU_freq: address to store the retrieved CPU frequency
* int *MEM_freq: address to store the retrieved memory frequency
* Returns 0 if there is no error.
*/
int get_CPU_freq_and_MEM_freq(	char *filename, 
								int *CPU_freq, 
								int *MEM_freq) 
{
	long int val;
	char *endptr;
	const char * separators = "_";
	char * strToken = strtok (filename, separators);
	// Integer counter in the filename
	int counter = 0;

	// For each part in the filename
    while ( strToken != NULL) {
		errno = 0;
		val = strtol(strToken, &endptr, 10);

		// Handle errors
		if (errno == ERANGE || errno == EINVAL) {
			fprintf(stderr, "strtol error: Invalid value\n");
        	return EXIT_FAILURE;
		}
		if (errno == ERANGE) {
			fprintf(stderr, "strtol error: Invalid range (too long or too short)\n");
        	return EXIT_FAILURE;
		}

		// If endptr is not at the beginning of the string, 
		// an integer value has been converted.
		if (endptr != strToken) {
			// By convention, the first value corresponds to the CPU frequency.
			if (!counter) {
				*CPU_freq = (int) val;
				counter++;
			}
			// By convention, the second value corresponds to the memory frequency.
			else if (counter == 1) {
				*MEM_freq = (int) val;
				counter++;
			}
			// Integer values not expected
			else {
				fprintf(stderr, "strtol error: Too much integer values.\n");
        		return EXIT_FAILURE;
			}
			
		}
		strToken = strtok (NULL, separators);
    }
	return 0;
}


int main()
{
	DIR *d1, *d2;
    struct dirent *dir1, *dir2;
    struct stat filestat1, filestat2;
	int CPU_freq, MEM_freq;
	// Path where the energy measures are located
	char path[64] = "results/energy_measures";

	// Open the directory containing energy measures of all programs.
	d1 = opendir(path);
    if (!d1) {
		fprintf(stderr, "opendir error: Unable to open directory %s\n", path);
        return EXIT_FAILURE;
	}

	// Check if it is a directory corresponding to one program.
	while ((dir1 = readdir(d1)) != NULL) {
		char dirname[MAX_LENGTH];
		sprintf(dirname, "%s/%s", path, dir1->d_name);
		stat(dirname, &filestat1);
		if( !(S_ISDIR(filestat1.st_mode) 
			&& strcmp(dir1->d_name, ".") 
			&& strcmp(dir1->d_name, "..")
			&& strcmp(dir1->d_name, "projects")
			&& strcmp(dir1->d_name, "backup")) )
			continue;
		printf("%s\n", dir1->d_name);

		// Open the directory containing energy measures of one program.
		d2 = opendir(dirname);
		if (!d2) {
			fprintf(stderr, "opendir error: Unable to open directory %s\n", dirname);
        	return EXIT_FAILURE;
		}

		// For each CSV file: get frequencies and apply some operations.
		while ((dir2 = readdir(d2)) != NULL) {
			char input_filename[MAX_LENGTH];
			sprintf(input_filename, "%s/%s/%s", path, dir1->d_name, dir2->d_name);
			stat(input_filename, &filestat2);
			if( !S_ISDIR(filestat2.st_mode) ) {
				get_CPU_freq_and_MEM_freq(dir2->d_name, &CPU_freq, &MEM_freq);
				operation(input_filename, 0, 0, begin_at_0);
				operation(input_filename, CPU_freq, MEM_freq, add_CPU_freq_MEM_freq_in_csv);
			}
		}

		// Close directories.
		closedir(d2);
	}
	closedir(d1);

	return 0;
}
