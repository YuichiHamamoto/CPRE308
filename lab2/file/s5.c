#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	int i = 0;
        if(fork() == 0){
		printf("Child starts\n");
                for(i = 0; i< 500000;i++){
                        printf("Child: %d\n",i);
                }
		printf("Child ends\n");

        }else{
		wait(NULL);
		printf("Parent starts\n");
              	for(i = 0; i<500000;i++){
                        printf("Parent: %d\n",i);
                }
		printf("Parent ends\n");
        }
	return 0;
}

