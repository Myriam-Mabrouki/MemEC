#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_LENGTH 1024

int begin_at_0(FILE *input_file, FILE *output_file, int _tmp, int _tmp2)
{
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

		if (lc == 1) {
			fputs(line, output_file);
			continue;
		}
		else if (lc == 2) {
			first_timestamp = timestamp;
		}

		// CPU and memory frequencies
		sprintf(newline, "%f,%f\n", timestamp - first_timestamp, value);
		// Write the modified line into the output file.
		fputs(newline, output_file);
	}

	return 0;
}

int add_fCPU_fMEM_in_csv(FILE *input_file, FILE *output_file, int fCPU, int fMEM)
{
	// Line counter
	int lc = 0;
	// Buffer to store each line of the input file.
	char line[MAX_LENGTH];
	// Buffer to store the addition going to the output file.
	char addition[MAX_LENGTH];

	// Read each line from the input file and store it in the 'line' buffer.
	while (fgets(line, sizeof(line), input_file)) {
		// Increments counter
		lc++;
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


int f(char *input_filename, int fCPU, int fMEM, int (*fct)(FILE*, FILE*, int, int))
{
	FILE* input_file = fopen(input_filename, "r");
	FILE* output_file = fopen("tmp", "w");
	if (input_file == NULL) {
		// Print an error message to the standard error stream if at least one file cannot be opened.
		fprintf(stderr, "Unable to open input file!\n");
		return EXIT_FAILURE;
	}
	if (output_file == NULL) {
		// Print an error message to the standard error stream if at least one file cannot be opened.
		fprintf(stderr, "Unable to open output file!\n");
		return EXIT_FAILURE;
	}
	//begin_at_0(input_file, output_file);
	fct(input_file, output_file, fCPU, fMEM);
	//rewind(input_file);
	// Close the file streams.
	fclose(input_file);
	fclose(output_file);
	// Replace original file with temporary file
	if (remove(input_filename) != 0) {
		fprintf(stderr, "Could not remove original file\n");
		remove("tmp");
		return 1;
	}

	if (rename("tmp", input_filename) != 0) {
		fprintf(stderr, "Could not rename temporary file\n");
		return 1;
	}
}

int main()
{
	DIR *d1, *d2;
    struct dirent *dir1, *dir2;
    struct stat filestat1, filestat2;
	char path[64] = "results/energy_measures";
	int fCPU, fMEM;

	d1 = opendir(path);
    if (!d1) {
		fprintf(stderr, "Unable to open directory %s\n", path);
        return EXIT_FAILURE;
	}
	while ((dir1 = readdir(d1)) != NULL) {
		char dirname[MAX_LENGTH];
		sprintf(dirname, "%s/%s", path, dir1->d_name);
		stat(dirname, &filestat1);
		if( !(S_ISDIR(filestat1.st_mode) && strcmp(dir1->d_name, ".") && strcmp(dir1->d_name, "..")) )
			continue;
		printf("%s\n", dir1->d_name);
		d2 = opendir(dirname);
		if (!d2) {
			fprintf(stderr, "Unable to open directory %s\n", dirname);
        	return EXIT_FAILURE;
		}
		while ((dir2 = readdir(d2)) != NULL) {
			char input_filename[MAX_LENGTH];
			sprintf(input_filename, "%s/%s/%s", path, dir1->d_name, dir2->d_name);
			stat(input_filename, &filestat2);
			if( !S_ISDIR(filestat2.st_mode) ) {
				char tmp[MAX_LENGTH];
				strcpy(tmp, dir2->d_name);
				strtok(tmp, "_");
				strtok(NULL, "_");
				strtok(NULL, "_");
				strtok(NULL, "_");
				fCPU = atoi(strtok(NULL, "_"));
				strtok(NULL, "_");
				fMEM = atoi(strtok(NULL, "_"));
				f(input_filename, fCPU, fMEM, begin_at_0);
				f(input_filename, fCPU, fMEM, add_fCPU_fMEM_in_csv);
			}
		}
		closedir(d2);
	}
	closedir(d1);
}
