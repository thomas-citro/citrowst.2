#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "config.h"


/* Global variables */
pid_t children[MAX_PROCS];
int nprocs;
int shmAllocated;
int shmid;
void *shmp;
char *programName;


/* Function Prototypes */
int isANumber(char*);
char *getOutputPerror();
int deallocateSharedMemory();
void ctrlCHandler(int);
static void myhandler(int);
static int setupinterrupt(void);
static int setupitimer(void);


int main (int argc, char *argv[]) {
	signal(SIGINT, ctrlCHandler);
	programName = argv[0];
	shmAllocated = 0;
	int ss = 100;
	nprocs = 0;
	
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
	
	shmAllocated = 1;
	shmid = shmget(SHM_KEY, SHM_SIZE, SHM_PERM|IPC_CREAT);
	if (shmid == -1) {
		char *output = getOutputPerror();
		perror(output);
		return 1;
	}
	
	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *) -1) {
		char *output = getOutputPerror();
		perror(output);
		return 1;
	}
	
	pid_t childpid = 0;
	int status = 0;
	if ((childpid = fork())) {
		/* parent process */
		printf("Parent -> Created child\n");
		if (setupinterrupt() == -1) {
			char *output = getOutputPerror();
			perror(output);
			return 1;
		}
		if (setupitimer() == -1) {
			char *output = getOutputPerror();
			perror(output);
			return 1;
		}

		wait(&status);
		printf("Parent -> All children have been terminated.\n");
	} else if (childpid < 0) {
		/* parent process with error */
		
	} else {
		/* child process */
		char *args[] = {"./slave", "1", (char*)0};
		printf("Child process (PID: %ld) in master program. About to execv to slave...\n", (long)getpid());
		execvp("./slave", args);
		perror("execv");
	}

	if (deallocateSharedMemory()) return 1;
	
        return 0;
}

int isANumber (char *str) {
	int i;
	for (i = 0; i < strlen(str); i++) {
		if (!isdigit(str[i])) return 0;
	}
	return 1;
}

char *getOutputPerror () {
	char* output = strdup(programName);
	strcat(output, ": Error");
	return output;
}

int deallocateSharedMemory() {
	int returnValue;
	printf("Opened deallocate function\n");
	returnValue = shmdt(shmp);
	if (returnValue == -1) {
		char *output = getOutputPerror();
		perror(output);
		return 1;
	}
	
	printf("Shared memory detached. Now deallocating/deleting from shared memory...\n");
	returnValue = shmctl(shmid, IPC_RMID, NULL);
	if (returnValue == -1) {
		char *output = getOutputPerror();
		perror(output);
		return 1;
	}

	return 0;
}

void ctrlCHandler (int dummy) {
	int i;
	for (i = 0; i < nprocs; i++) {
		if ((kill(children[i], SIGKILL)) == -1) {
			char *output = getOutputPerror();
			perror(output);
		}
	}
	if (shmAllocated) deallocateSharedMemory();
	exit(0);
}

static int setupitimer(void) {
	struct itimerval value;
	value.it_interval.tv_sec = 2;
	value.it_interval.tv_usec = 0;
	value.it_value = value.it_interval;
	return (setitimer(ITIMER_PROF, &value, NULL));
}

static int setupinterrupt(void) {
	struct sigaction act;
	act.sa_handler = myhandler;
	act.sa_flags = 0;
	return (sigemptyset(&act.sa_mask) || sigaction(SIGPROF, &act, NULL));
}

static void myhandler(int s) {
	fprintf(stdout, "Inside my timer handler\n");
	char aster = '*';
	int errsave;
	errsave = errno;
	write(STDERR_FILENO, &aster, 1);
	errno = errsave;
}
