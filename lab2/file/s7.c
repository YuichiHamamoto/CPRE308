#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	execl("/bin/ls","ls",(char *)NULL);
	printf("What happened?\n");
}
