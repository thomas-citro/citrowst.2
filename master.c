#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "config.h"

/* Function Prototypes */
int isANumber(char*);
char *getOutputPerror(char*);

int main (int argc, char *argv[]) {	
	int ss = 100;
	int nprocs;
	
	int usageStatement = 0;
	int option;
	while ((option = getopt(argc, argv, "ht:")) != -1) {
		switch (option) {
			case 'h' :
				usageStatement = 1;
				break;
			case 't' :
				if (!isANumber(optarg)) {
					usageStatement = 1;
					break;
				} else {
					printf("Hello, you chose 't' as parameter with value: %s\n", optarg);
					ss = atoi(optarg);
				}
				break;
			default:
				printf("Error\n");
		}
	}
	if (argv[optind] == NULL) {
		usageStatement = 1;
	} else {
		if (!isANumber(argv[optind])) usageStatement = 1;
		else {
			/* TODO: Check to see if n is over 20 */
			printf("The value you chose for 'n' is: %s\n", argv[optind]);
			nprocs = 1; /* Temporarily */
		}
	}
	if (usageStatement) {
		printf("Usage: ./master [-h] [-t ss] [n]\n");
		printf("Runs master program to fork 'n' slave processes.\n");
		printf("Options:\n");
		printf("-h      for help.\n");
		printf("-t ss   (integer) sets the maximum time in seconds (default 100) after which the process should terminate itself if not completed.\n");
		printf("n       (integer) sets the number of processes to fork off (max 20).\n");
		return 0;
	}	
	
	int shmid;
	void *shmp;
	shmid = shmget(SHM_KEY, SHM_SIZE, SHM_PERM|IPC_CREAT);
	if (shmid == -1) {
		char *output = getOutputPerror(argv[0]);
		perror(output);
		return 1;
	}
	
	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *) -1) {
		char *output = getOutputPerror(argv[0]);
		perror(output);
		return 1;
	}
	if (deallocateSharedMemory(shmid, shmp, argv[0])) return 1;
	
        return 0;
}

int isANumber (char *str) {
	int i;
	for (i = 0; i < strlen(str); i++) {
		if (!isdigit(str[i])) return 0;
	}
	return 1;
}

char *getOutputPerror (char *str) {
	char* output = strdup(str);
	strcat(output, ": Error");
	return output;
}

int deallocateSharedMemory(int shmid, void *shmp, char *programName) {
	int returnValue;
	printf("Opened deallocate function\n");
	returnValue = shmdt(shmp);
	if (returnValue == -1) {
		char *output = getOutputPerror(strdup(programName));
		perror(output);
		return 1;
	}
	printf("Marker 4\n");
	
	printf("Shared memory detached. Now deallocating/deleting from shared memory...\n");
	returnValue = shmctl(shmid, IPC_RMID, NULL);
	if (returnValue == -1) {
		char *output = getOutputPerror(programName);
		perror(output);
		return 1;
	}

	return 0;
}
