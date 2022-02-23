#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main (int argc, char *argv[]) {
	printf("Hello, I am the slave process with process number (xx): %d...\n My pid is: %ld. And my parent's pid is: %ld.\n", atoi(argv[1]), (long)getpid(), (long)getppid());
	sleep(5);
	
        return 0;
}

