#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int ret;
	ret=fork();
	if(ret==0){
		printf("The child process ID is %d\n",getpid());
		printf("The child's parent process ID is %d\n",getppid());
	}else{
		printf("The parent process ID is %d\n",getpid());
                printf("The parent's parent process ID is %d\n",getppid());
	}
	sleep(2);
	return 0;	
        
}


