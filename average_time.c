#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 1024

int main()
{
	// Create a file pointer and open the file ".txt" in read mode.
	FILE* input_file = fopen("results/time_measures/statemate/statemate_time_measure_fCPU_1400_fMEM_500.txt", "r");
	// Create a file pointer and open the file ".txt" in write mode. 
	// If it already exists, the file will be overwritten.
	FILE* output_file = fopen("results/time_measures/statemate/statemate_average_execution_time.txt", "w");
	// Print the header to the output file
	fprintf(output_file, "exec_time CPU_freq memory_freq\n");
	

	// Buffer to store each line of the file.
	char line[MAX_LENGTH];
	// Line counter
	int lc = 0;
	// Value counter
	int vc = 0;
	// Sum of execution time values
	long double sum = 0;
	// CPU and memory frequencies
	int fCPU = 1400, fMEM = 500;

	// Check if the file was opened successfully.
	if (input_file != NULL) {
		// Read each line from the file and store it in the 'line' buffer.
		while (fgets(line, sizeof(line), input_file)) {
			//Increment line counter
			lc++;
			// Check if the content of the current line corresponds to elapsed time
			if (lc % 4 == 0) {
				// Convert the string into a float and add it to the sum of execution time values
				sum += atof(line);
				//Increment value counter
				vc++;
			}
		}

		// Check if there is at least one value
		if (vc != 0) {
			// Compute the average execution time
			long double average = (long double) sum / vc;
			// Print the result to the output file
			fprintf(output_file, "%Lf %d %d\n", average, fCPU, fMEM);
		}

		// Close the input file stream once all lines have been read.
		fclose(input_file);
		// Close the output file stream once all results have been written.
		fclose(output_file);
		}

	else {
		// Print an error message to the standard error stream if the file cannot be opened.
		fprintf(stderr, "Unable to open file!\n");
		// End the program with an error
		exit(1);
	}

	// End the program successfully
	return 0;
}