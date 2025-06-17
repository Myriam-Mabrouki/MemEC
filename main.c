#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	const int MAX_LOOP = 100000;
	const int MEM_SIZE = 1024*1024;
	const int REPORT_FREQ = 1;

	int i, j;
	char **memoryArray;
	memoryArray = malloc(MAX_LOOP * sizeof(char *));

	for (i=0; i<=MAX_LOOP; i++){
		memoryArray[i] = malloc(MEM_SIZE);
		for (j=0; j<MEM_SIZE; j++) {
			memoryArray[i][j] = '\0';
		}

		if (i%100 == 0) {
			printf("LOOP ITERATION: %d\n\tMEMORY USAGE: %dMB\n", i, i);
		}
	}

	printf("LOOP FINISHED: WAITING\n");
	for(;;){}
}
