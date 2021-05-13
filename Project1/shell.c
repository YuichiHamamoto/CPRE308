#include <stdlib.h> 
#include <stdio.h> 
#include <signal.h> 
#include <string.h>

#define MAXINPUTSIZE 256

int main(int argc, char **argv){
    // Set the prompt
    char* prompt = "308sh> ";
    
    char input[MAXINPUTSIZE];
    char dir[MAXINPUTSIZE];
    char cwd[1024];

	int status;

	int countArg;

    //0 indicates in child processes
	int child;
	int child2;

	int backgroundProcess = 0;

	//Counters
    int i,j;

	//Check if the argument is the right size
	if(argc!=3&&argc!=1){
		printf("Invalid command. Exiting.\n");
		return 0;
	}

	//check for prompt
	if(argc == 3)
	{
		//The first argument is not -p
		if((strlen(argv[1]) != 2) || (strncmp("-p", argv[1], 2) != 0))
		{
			printf("Invalid command. Exiting.\n");
			return 0;
		}
		//assign prompt argument to prompt variable
		prompt = argv[2];
	}

    // User input infinite loop
    while(1)
	{
		printf(prompt);
        // Get user input
		fgets (input, MAXINPUTSIZE, stdin);
        
		if ((strlen(input)>0) && (input[strlen (input) - 1] == '\n'))
		{
        		input[strlen(input) - 1] = '\0';
		}
        // ---------- Builtin Commands ---------------------------------------	
        //exit
        if (strcmp(input, "exit") == 0) {
            return 0;
        } 
        //pid
        else if (strcmp(input, "pid") == 0) {
            printf("Shell Process ID: %d\n", getpid()); 
        }
        //ppid
        else if(strcmp(input, "ppid") == 0){
            printf("Process ID of this shell's parent: %d\n", getppid());
        }
        //cd
        else if(strncmp("cd", input, 2) == 0 && (strlen(input) == 2 || input[2] == ' ')){
            if(strlen(input)==2 ){
                chdir(getenv("HOME"));
            }
            else{
                for(i=0;i<strlen(input)-3;i++){
                    dir[i] = input[i+3];
                    dir[i+1] = '\0';
                }
				if(chdir(dir) == -1)
				{
					printf("Cannot find directory\n");
				}
            }
        }
        //pwd
        else if(strncmp("pwd",input,3) == 0){
            if(getcwd(cwd, 1024)==-1){
                printf("Fail on pwd");
            }
            else{
                printf("%s\n", cwd);
            }
        }
		
        else {
            // ---------- Non-Builtin Commands--------------------------
            //count of arguments for args[]
			countArg = 1;
			for(i = 0; i < strlen(input); i++)
			{
				if (input[i] == ' ')
				{
					countArg++;
				}
			}

			//check to see if last argument is '&' 
			if(input[strlen(input) - 1] == '&')
			{
				countArg--;
				backgroundProcess = 1;
			}
			//fill args[] with each argument
			char temp[countArg][MAXINPUTSIZE];
			char* args[countArg + 1];
			countArg = 0;
			int k = 0;
			for(i = 0; i < strlen(input) + 1; i++)
			{
				if (input[i] == ' ' || input[i] == '\0')
				{
					temp[countArg][k] = '\0';
					args[countArg] = temp[countArg];
					countArg++;
					k = 0;
				}
				else if (input[i] == '&' && i == strlen(input) - 1)
				{
					i = strlen(input) + 1;
				}
				else
				{
					temp[countArg][k] = input[i];
					k++;
				}
			}
			
			args[countArg] = (char*) NULL;
            
			/* If background process spawn a child(child2), it will spawn child(child) to run the process.
			*child2 wait for child to compelete.
			*/
			if(backgroundProcess == 1)
			{
				backgroundProcess = 0;
				child2 = fork();
				if(child2 == 0)
				{
					child = fork();
					if(child == 0)
					{
						//print process ID and name of command
						printf("[%d] %s\n", getpid(), args[0]);
						execvp(args[0], args);
						perror("\0");
						return 0;
					}
					else
					{
						status = -1;
						waitpid(-1, &status, 0);
						printf("\n[%d] %s Exit %d\n", child, args[0], WEXITSTATUS(status));
						printf(prompt);
						return 0;
					}
				}
				else
				{
					usleep(1000);
				}
			}
			//spawn child (child) that runs command and wait to complete before accepting more input
			else
			{
				child = fork();
				//in child process
				if(child == 0)
				{
					//print process ID and name of command
					printf("[%d] %s\n", getpid(), args[0]);
					execvp(args[0], args);
					perror("\0");
					return 0;
				}
				else
				{
					usleep(1000);
					status = -1;
					waitpid(child, &status, 0);
					//print the completed child process ID name and return status
					printf("\n[%d] %s Exit %d\n", child, args[0], WEXITSTATUS(status));
				}
			}
        }	
    }
    return 0;
}
