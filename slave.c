#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

int main (int argc, char *argv[]) {
	int procNumber = atoi(argv[1]);
	char logfile[10] = "logfile.";
	strcat(logfile, argv[1]);
	
	printf("Hello, I am a slave process with process number (xx): %d... My pid is: %ld. And my parent's pid is: %ld.\n", procNumber, (long)getpid(), (long)getppid());
	FILE *fptr = fopen(logfile, "w");
	if (fptr == NULL) {
		printf("Slave (w/ process number %d) is unable to open their log file called: %s\n", procNumber, logfile);
		return 0;
	}
	
	fprintf(fptr, "I am slave w/ process number %d. Pid is: %ld.\n", procNumber, (long)getpid());
	fclose(fptr);
	
	sleep(5);
	
        return 0;
}

