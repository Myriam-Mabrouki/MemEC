#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_LENGTH 1024

//int f(int MIN_CPU, int MAX_CPU, int STEP_CPU, int MIN_MEM, int MAX_MEM, int STEP_MEM, ptr de fonction)

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
                sprintf(output_filename, "results/time_measures/%s/%s_results.txt", dir1->d_name, dir1->d_name);
                FILE* output_file = fopen(output_filename, "w");
                d2 = opendir(dirname);
                if (d2) {
                    while ((dir2 = readdir(d2)) != NULL) {
                        char filename[MAX_LENGTH];
                        sprintf(filename, "results/time_measures/%s/%s", dir1->d_name, dir2->d_name);
                        stat(filename, &filestat2);
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
