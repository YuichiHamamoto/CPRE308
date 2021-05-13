#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	fork();
	fork();
	usleep(1);
	printf("Process %d's parent ID is %d\n",getpid(),getppid());
	sleep(2);	
	return 0;
}
