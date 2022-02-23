#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

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
	
	
	printf("Hello, I am a slave process with process number (xx): %d... My pid is: %ld. And my parent's pid is: %ld.\n", procNumber, (long)getpid(), (long)getppid());
	FILE *fptr = fopen(logfile, "w");
	if (fptr == NULL) {
		printf("Slave (w/ process number %d) is unable to open their log file called: %s\n", procNumber, logfile);
		return 0;
	}
	
	fprintf(fptr, "I am slave w/ process number %d. Pid is: %ld.\n", procNumber, (long)getpid());
	fclose(fptr);
	
	
	srand(time(0) * procNumber); /* Seed random number generator */
	
	int randomNum = (rand() % (randUpper - randLower + 1)) + randLower; /* Chooses random number from randLower to randUpper */
	
	sleep(randomNum);
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	printf("Slave (proc num: %d). System time (hh:mm:ss) -> %d:%d:%d\n", procNumber, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec); 
	
        return 0;
}

