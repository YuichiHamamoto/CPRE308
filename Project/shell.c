
#include <stdlib.h> 
#include <stdio.h> 
#include <signal.h> 
#include <string.h>

#define MAXINPUTSIZE 100

int main(int argc, char **argv){
    char* prompt = "308sh> ";
    char input[MAXINPUTSIZE];
    while(1)
	{
		/*prompt and wait for input*/
		printf(prompt);

        /*get user input and ensure it is null terminated*/
		fgets (input, MAXINPUTSIZE, stdin);
		if ((strlen(input)>0) && (input[strlen (input) - 1] == '\n'))
		{
        		input[strlen(input) - 1] = '\0';
		}	
        if (strcmp(input, "exit") == 0) {
            return 0;
        } 
        else if (strcmp(input, "pid") == 0) {
            printf("Shell Process ID: %d\n", getpid()); 
        }

        else {
            nonBuiltIn(input); 
        }	
    }
    return 0;
}

// ----------------------------------- Helper Functions ---------------------------------- 
int nonBuiltIn(char input[]) {
    
}