#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

int main (int argc, char *argv[]) {
	
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
	int rand2 = (rand() % (randUpper - randLower + 1)) + randLower;
	int rand3 = (rand() % (randUpper - randLower + 1)) + randLower;
	int rand4 = (rand() % (randUpper - randLower + 1)) + randLower;
	int rand5 = (rand() % (randUpper - randLower + 1)) + randLower;
	int rand6 = (rand() % (randUpper - randLower + 1)) + randLower;
	
	printf("Slave (w/ proc num: %d) has sleep values: %d, %d, %d, %d, %d, %d\n", procNumber, randomNum, rand2, rand3, rand4, rand5, rand6);
	sleep(randomNum);
	
        return 0;
}

