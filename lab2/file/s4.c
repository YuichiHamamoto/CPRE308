#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int i = 0;
	int pid = fork();
	if(pid == 0){
		for(i = 0; i< 500000;i++){
			printf("Child: %d\n",i);
		}
	}else{
		for(i = 0; i<500000;i++){
			printf("Parent: %d\n",i);
		}
	}
}

