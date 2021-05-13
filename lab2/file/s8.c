#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int status;
	if(fork() == 0){
		srand(time(NULL));
		if(rand()<RAND_MAX/4){
			kill(getpid(),SIGTERM);
		}
		return rand();
	}else{
		wait(&status);
			if (WIFEXITED(status)) {
				printf("Child exited with status %d\n", WEXITSTATUS(status));
			} else if (WIFSIGNALED(status)) {
				printf("Child exited with signal %d\n", WTERMSIG(status));
			}
		}
		return 0;
	}
