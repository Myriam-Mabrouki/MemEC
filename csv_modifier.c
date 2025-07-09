#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1024

int main()
{
	// CPU and memory frequencies
	int fCPU, fMEM = 500;
	// Number of executions
	int N = 500;
	// Averages execution times
	float avg1 = 0.01, avg2 = 0.008, avg3 = 0.003;
	// Intervals
	float begin1 = 0, end1 = N * avg1, begin2 = end1 + 2, end2 = begin2 + N * avg2, begin3 = end2 + 3, end3 = begin3 + N * avg3;

	// Create a file pointer and open the file "test" in read mode.
	FILE* input_file = fopen("test", "r");
	// Create a file pointer and open the file "res" in write mode.
	FILE* output_file = fopen("test_modified", "w");
	// Check if the files were opened successfully.
	if (input_file == NULL || output_file == NULL)
	{
		// Print an error message to the standard error stream if at least one file cannot be opened.
		fprintf(stderr, "Unable to open file!\n");
	}

	// Line counter
	int lc = 0;
	// Buffer to store each line of the input file.
	char line[MAX_LENGTH];
	// Buffer to store the addition going to the output file.
	char addition[MAX_LENGTH];
	// Timestamp for power consumption
	float timestamp;

	// Read each line from the input file and store it in the 'line' buffer.
	while (fgets(line, sizeof(line), input_file)) {
		// Increments counter
		lc++;
		char tmp[MAX_LENGTH];
        strcpy(tmp, line);
        timestamp = atof(strtok(tmp, ","));
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

	// Close the file streams.
	fclose(input_file);
	fclose(output_file);

	return 0;
}