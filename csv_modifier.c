#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1024

int main()
{
	// Create a file pointer and open the file "test" in read mode.
	FILE* input_file = fopen("test", "r");
	// Create a file pointer and open the file "res" in write mode.
	FILE* output_file = fopen("res", "w");
	// Check if the files were opened successfully.
	if (input_file == NULL || output_file == NULL)
	{
		// Print an error message to the standard error stream if at least one file cannot be opened.
		fprintf(stderr, "Unable to open file!\n");
	}

	// CPU and memory frequencies
	int fCPU = 1400, fMEM = 500;

	// Buffer to store each line of the input file.
	char line[MAX_LENGTH];
	// Buffer to store the addition going to the output file.
	char addition[MAX_LENGTH];

	// Read each line from the input file and store it in the 'line' buffer.
	while (fgets(line, sizeof(line), input_file)) {
		// Remove the newline character.
		line[strcspn(line, "\n")] = 0;
		// Create the additional part (CPU and memory frequencies).
		sprintf(addition, " %d %d\n", fCPU, fMEM);
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