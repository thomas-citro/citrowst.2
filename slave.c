#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <sys/shm.h>
#include "config.h"


/* Function prototypes */
void lock(int);
void use_resource(int);
void unlock(int);
char *getOutputPerror(char*);


struct shmseg {
	int resource;
	int tickets[MAX_PROCS];
	int choosing[MAX_PROCS];
};
struct shmseg *shmp;


int main (int argc, char *argv[]) {
	time_t rawtime;
	struct tm * timeinfo;
	int randLower = 1;
	int randUpper = 5;
	int procNumber = atoi(argv[1]) + 1;
	char intToString[3];
	sprintf(intToString, "%d", procNumber);
	char logfile[10] = "logfile.";
	strcat(logfile, intToString);
	srand(time(0) * procNumber); /* Seed the random number generator */
	int randomNum;
	int shmid = atoi(argv[2]);
	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *) -1) {
		char *output = getOutputPerror(argv[0]);
		perror(output);
		exit(1);
	}
	
	/* Bakery's algorithm implementation */
	int i = 0;
	for (i = 0; i < 5; i++) {
		lock(procNumber - 1); /* Enter critical section */
		randomNum = (rand() % (randUpper - randLower + 1)) + randLower; /* Random number from randLower to randUpper */
		sleep(randomNum);
		use_resource(procNumber - 1); /* Critical section */
		randomNum = (rand() % (randUpper - randLower + 1)) + randLower; /* Random number from randLower to randUpper */
		sleep(randomNum);
		unlock(procNumber - 1); /* Exit critical section */
	}
	printf("Out of for loop for bakery algorithm\n");
	
	
	
	
	
	
	
	/*
	printf("Hello, I am a slave process with process number (xx): %d... My pid is: %ld. And my parent's pid is: %ld.\n", procNumber, (long)getpid(), (long)getppid());
	FILE *fptr = fopen(logfile, "w");
	if (fptr == NULL) {
		printf("Slave (w/ process number %d) is unable to open their log file called: %s\n", procNumber, logfile);
		return 0;
	}
	
	fprintf(fptr, "I am slave w/ process number %d. Pid is: %ld.\n", procNumber, (long)getpid());
	fclose(fptr);
	
	
	srand(time(0) * procNumber);
	
	int randomNum = (rand() % (randUpper - randLower + 1)) + randLower;
	
	sleep(randomNum);
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	printf("Slave (proc num: %d). System time (hh:mm:ss) -> %d:%d:%d\n", procNumber, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec); 
	*/
        return 0;
}

void lock(int procNumber) {
	/* Before getting the ticket number, "choosing" variable is set to be true. */
	shmp->choosing[procNumber] = 1;
	MEMBAR; /* Memory barrier applied */
	
	int max_ticket = 0;
	
	/* Finding Maximum ticket value among current threads */
	int i;
	for (i = 0; i < MAX_PROCS; ++i) {
		int ticket = shmp->tickets[i];
		max_ticket = ticket > max_ticket ? ticket : max_ticket;
	}
	
	/* Allotting a new ticket value as MAXIMUM + 1 */
	shmp->tickets[procNumber] = max_ticket + 1;
	
	MEMBAR;
	shmp->choosing[procNumber] = 0;
	MEMBAR;
	
	/* The ENTRY Section starts from here */
	int other;
	for (other = 0; other < MAX_PROCS; ++other) {
		
		/* Applying the bakery algorithm conditions */
		while (shmp->choosing[other]) {
		}
		
		MEMBAR;
		
		while (shmp->tickets[other] != 0 && (shmp->tickets[other] < shmp->tickets[procNumber] || (shmp->tickets[other] == shmp->tickets[procNumber] && other < procNumber))) {
		}
		
	}
}

/* EXIT Section */
void unlock(int procNumber) {
	MEMBAR;
	shmp->tickets[procNumber] = 0;
}


/* The CRITICAL Section */
void use_resource(int procNumber) {
	if (shmp->resource != 0) {
		printf("Resource was acquired by %d, but is still in-use by %d!\n", procNumber, shmp->resource);
	}
	
	shmp->resource = procNumber;
	printf("%d using resource...\n", procNumber);
	
	MEMBAR;
	
	shmp->resource = 0;
}

char *getOutputPerror(char *programName) {
	char* output = strdup(programName);
	strcat(output, ": Error");
	return output;
}
