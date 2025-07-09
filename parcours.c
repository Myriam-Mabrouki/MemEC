#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#define MAX_LENGTH 1024

//int f(int MIN_CPU, int MAX_CPU, int STEP_CPU, int MIN_MEM, int MAX_MEM, int STEP_MEM, ptr de fonction)

int main()
{
    DIR *d;
    struct dirent *dir;
    struct stat filestat;

    d = opendir("results/time_measures");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            stat(dir->d_name,&filestat);
            if( S_ISDIR(filestat.st_mode) && strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") ) {
                int fCPU, fMEM, MIN_CPU = 600, MAX_CPU = 1400, STEP_CPU = 400, MIN_MEM = 200, MAX_MEM = 500, STEP_MEM = 100;
            	for (fCPU = MIN_CPU; fCPU <= MAX_CPU; fCPU=fCPU+STEP_CPU) {
            	    for (fMEM = MIN_MEM; fMEM <= MAX_MEM; fMEM=fMEM+STEP_MEM) {
                    	char filename[MAX_LENGTH]="results/time_measures/";
                    	char addition[MAX_LENGTH];
            		    sprintf(addition, "%s/%s_time_measure_fCPU_%d_fMEM_%d", dir->d_name, dir->d_name, fCPU, fMEM);
            	        strcat(filename, addition);
            	        printf("%s\n", filename);
            	    }
            	}
            }
            
        }
        closedir(d);
    } else {
        fprintf(stderr, "Unable to open directory");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
