#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_LENGTH 1024

int average_execution_time(FILE* input_file, FILE* output_file, int fCPU, int fMEM)
{
	// Buffer to store each line of the file.
	char line[MAX_LENGTH];
	// Line counter
	int lc = 0;
	// Value counter
	int vc = 0;
	// Sum of execution time values
	long double sum = 0;

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


	// End the program successfully
	return 0;
}

int main()
{
    DIR *d1, *d2;
    struct dirent *dir1, *dir2;
    struct stat filestat1, filestat2;
    int fCPU, fMEM;

    d1 = opendir("results/time_measures");
    if (d1) {
        while ((dir1 = readdir(d1)) != NULL) {
            char dirname[MAX_LENGTH];
            sprintf(dirname, "results/time_measures/%s", dir1->d_name);
            stat(dirname, &filestat1);
            if( S_ISDIR(filestat1.st_mode) && strcmp(dir1->d_name, ".") && strcmp(dir1->d_name, "..") ) {
                printf("%s\n", dirname);
                char output_filename[MAX_LENGTH];
                sprintf(output_filename, "results/time_measures/%s/%s_results_execution_time.txt", dir1->d_name, dir1->d_name);
                FILE* output_file = fopen(output_filename, "w");
                // Print the header to the output file
	            fprintf(output_file, "exec_time CPU_freq memory_freq\n");
                d2 = opendir(dirname);
                if (d2) {
                    while ((dir2 = readdir(d2)) != NULL) {
                        char filename[MAX_LENGTH];
                        sprintf(filename, "results/time_measures/%s/%s", dir1->d_name, dir2->d_name);
                        stat(filename, &filestat2);
                        //TODO : check que le début de la chaîne démarre avec {dir1->d_name}_time_measure
                        if( !S_ISDIR(filestat2.st_mode) && strcmp(filename, output_filename)) {
                            printf("%s\n", filename);
                            FILE* input_file = fopen(filename, "r");
                            if (input_file) {
                                char tmp[MAX_LENGTH];
                                strcpy(tmp, dir2->d_name);
                                strtok(tmp, "_");
                                strtok(NULL, "_");
                                strtok(NULL, "_");
                                strtok(NULL, "_");
                                fCPU = atoi(strtok(NULL, "_"));
                                strtok(NULL, "_");
                                fMEM = atoi(strtok(NULL, "_"));
                                printf("fCPU %d, fMEM %d\n", fCPU, fMEM);
                                average_execution_time(input_file, output_file, fCPU, fMEM);
                                fclose(input_file);
                            } else {
                                // Print an error message to the standard error stream if the file cannot be opened.
                                fprintf(stderr, "Unable to open file!\n");
                                // End the program with an error
                                return EXIT_FAILURE;
                            }
                        }
                    }
                    closedir(d2);
                    fclose(output_file);
                } else {
                    fprintf(stderr, "Unable to open directory %s\n", dirname);
                    return EXIT_FAILURE;
                }
            }
            
        }
        closedir(d1);
    } else {
        fprintf(stderr, "Unable to open directory results/time_measures");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
