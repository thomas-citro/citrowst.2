#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
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
	if (usageStatement) {
		printf("Usage: ./master [-h] [-t ss] [n]\n");
		printf("Runs master program to fork 'n' slave processes.\n");
		printf("Options:\n");
		printf("-h      for help.\n");
		printf("-t ss   (integer) sets the maximum time in seconds (default 100) after which the process should terminate itself if not completed.\n");
		printf("n       (integer) sets the number of processes to fork off (max 20).\n");
		return 0;
	}
	if (argv[optind] == NULL) {
		printf("You did not choose any value for 'n'\n");
		/* printf("For testing purposes, setting nprocs to 1\n"); */
		nprocs = 1;
	} else {
		printf("The value you chose for 'n' is: %s\n", argv[optind]);
		if (!isANumber(argv[optind])) {
			fprintf(stderr, "%s: Error: Invalid input\n", argv[0]);
			return 0;
		}
		/* printf("For testing purposes, setting nprocs to 1\n"); */
		nprocs = 1;
	}

        return 0;
}

int isANumber (char *str) {
	int i;
	for (i = 0; i < strlen(str); i++) {
		if (!isdigit(str[i])) return 0;
	}
	return 1;
}
