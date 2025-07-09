#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1024

int main()
{
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
	// Buffer to store the new line going to the output file.
	char newline[MAX_LENGTH];
	// Timestamp for power consumption
	float timestamp, first_timestamp, value;

	// Read each line from the input file and store it in the 'line' buffer.
	while (fgets(line, sizeof(line), input_file)) {
		// Increments counter
		lc++;
		char tmp[MAX_LENGTH];
        strcpy(tmp, line);
        timestamp = atof(strtok(tmp, ","));
		value = atof(strtok(NULL, ","));

		if (lc == 1)
			continue;
		else if (lc == 2)
			first_timestamp = timestamp;

		// CPU and memory frequencies
		sprintf(newline, "%f,%f\n", timestamp - first_timestamp, value);
		// Write the modified line into the output file.
		fputs(newline, output_file);
	}

	// Close the file streams.
	fclose(input_file);
	fclose(output_file);

	return 0;
}