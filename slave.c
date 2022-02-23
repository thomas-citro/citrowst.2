#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include "config.h"

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
	
	
	
	
	/* Bakery's algorithm implementation */
	int i = 0;
	for (i = 0; i < 5; i++) {
		lock(procNumber); /* Enter critical section */
		randomNum = (rand() % (randUpper - randLower + 1)) + randLower; /* Random number from randLower to randUpper */
		sleep(randomNum);
		use_resource(procNumber); /* Critical section */
		randomNum = (rand() % (randUpper - randLower + 1)) + randLower; /* Random number from randLower to randUpper */
		sleep(randomNum);
		unlock(procNumber); /* Exit critical section */
	}
	
	
	
	
	
	
	
	
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
	choosing[procNumber] = 1;

	MEMBAR; /* Memory barrier applied */
	
	int max_ticket = 0;
	
	/* Finding Maximum ticket value among current threads */
	int i;
	for (i = 0; i < MAX_PROCS; ++i) {
		int ticket = tickets[i];
		max_ticket = ticket > max_ticket ? ticket : max_ticket;
	}
	
	/* Allotting a new ticket value as MAXIMUM + 1 */
	tickets[procNumber] = max_ticket + 1;

	MEMBAR;
	choosing[procNumber] = 0;
	MEMBAR;
	
	/* The ENTRY Section starts from here */
	int other;
	for (other = 0; other < MAX_PROCS; ++other) {
		
		/* Applying the bakery algorithm conditions */
		while (choosing[other]) {
		}
		
		MEMBAR;
		
		while (tickets[other] != 0 && (tickets[other] < tickets[procNumber] || (tickets[other] == tickets[procNumber] && other < procNumber))) {
		}
		
	}
}

/* EXIT Section */
void unlock(int procNumber) {
	MEMBAR;
	tickets[procNumber] = 0;
}


/* The CRITICAL Section */
void use_resource(int procNumber) {
	if (resource != 0) {
		printf("Resource was acquired by %d, but is still in-use by %d!\n", procNumber, resource);
	}
	
	resource = procNumber;
	printf("%d using resource...\n", procNumber);
	
	MEMBAR;
	
	resource = 0;
}
