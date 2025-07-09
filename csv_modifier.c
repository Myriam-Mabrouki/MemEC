#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_LENGTH 1024

int add_FCPU_fMEM_in_csv(FILE *input_file, FILE *output_file) 
{
  // CPU and memory frequencies
	int fCPU, fMEM = 500;
	// Number of executions
	int N = 500;
	// Averages execution times
	float avg1 = 0.01, avg2 = 0.008, avg3 = 0.003;
	// Intervals
	float begin1, end1, begin2, end2, begin3, end3;

	// Line counter
	int lc = 0;
	// Buffer to store each line of the input file.
	char line[MAX_LENGTH];
	// Buffer to store the addition going to the output file.
	char addition[MAX_LENGTH];
	// Timestamp for power consumption
	float timestamp;
	// Value of power consumption
	float value;

	// Read each line from the input file and store it in the 'line' buffer.
	while (fgets(line, sizeof(line), input_file)) {
		// Increments counter
		lc++;
		char tmp[MAX_LENGTH];
        strcpy(tmp, line);
        timestamp = atof(strtok(tmp, ","));

		if (begin1 == -1 && lc != 1) {
			value = atof(strtok(NULL, ","));
			if (value >= 2.5) {
				begin1 = timestamp;
				end1 = begin1 + N * avg1;
				begin2 = end1 + 2;
				end2 = begin2 + N * avg2;
				begin3 = end2 + 2;
				end3 = begin3 + N * avg3;
			} else {
				timestamp = -2;
			}
		}
		
		if (begin1 <= timestamp && timestamp <= end1)
			fCPU = 600;
		else if (begin2 <= timestamp && timestamp <= end2)
			fCPU = 1000;
		else if (begin3 <= timestamp && timestamp <= end3)
			fCPU = 1400;
		else
			fCPU = -1;
		
		// Remove the newline character.
		line[strcspn(line, "\n")] = 0;
		// Create the additional part
		if (lc == 1)
			// Special case for the header
			sprintf(addition, ",\"CPU freq\",\"Memory freq\"\n");
		else
			// CPU and memory frequencies
			sprintf(addition, ",%d,%d\n", fCPU, fMEM);
		// Concatenate the additional part to the input line.
		strcat(line, addition);
		// Write the modified line into the output file.
		fputs(line, output_file);
	}
	
	return 0;
}

int main()
{
	DIR *d1, *d2;
    struct dirent *dir1, *dir2;
    struct stat filestat1, filestat2;

	d1 = opendir("results/energy_measures");
    if (!d1) {
		fprintf(stderr, "Unable to open directory results/energy_measures\n");
        return EXIT_FAILURE;
	}
	while ((dir1 = readdir(d1)) != NULL) {
		char dirname[MAX_LENGTH];
		sprintf(dirname, "results/energy_measures/%s", dir1->d_name);
		stat(dirname, &filestat1);
		//TODO: regex plutôt que la dernière condition
		if( !(S_ISDIR(filestat1.st_mode) && strcmp(dir1->d_name, ".") && strcmp(dir1->d_name, "..") && strcmp(dir1->d_name, "statemate_modified")) )
			continue;
		printf("%s\n", dir1->d_name);
		d2 = opendir(dirname);
		if (!d2) {
			fprintf(stderr, "Unable to open directory %s\n", dirname);
        	return EXIT_FAILURE;
		}
		while ((dir2 = readdir(d2)) != NULL) {
			char input_filename[MAX_LENGTH];
			sprintf(input_filename, "results/energy_measures/%s/%s", dir1->d_name, dir2->d_name);
			stat(input_filename, &filestat2);
			if( !S_ISDIR(filestat2.st_mode) ) {
				FILE* input_file = fopen(input_filename, "r");
				char output_filename[MAX_LENGTH];
				sprintf(output_filename, "results/energy_measures/%s_modified/modified_%s", dir1->d_name, dir2->d_name);
				FILE* output_file = fopen(output_filename, "w");
				if (input_file == NULL || output_file == NULL) {
					// Print an error message to the standard error stream if at least one file cannot be opened.
					fprintf(stderr, "Unable to open file!\n");
				}
				add_FCPU_fMEM_in_csv(input_file, output_file);
				// Close the file streams.
				fclose(input_file);
				fclose(output_file);
			}
		}
	}
	return 0;
}