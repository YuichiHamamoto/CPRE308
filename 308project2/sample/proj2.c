

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Bank.h"
#include "Bank.c"
#include <string.h>
#include <sys/time.h>



//to handle a mutext for each account
struct account {
  pthread_mutex_t lock;
  int value;
};

//for requests
struct Request  
{ 
  char* whattodo[1024];
  struct timeval time;
  int request_id;
};

//for keeping track of requests 
struct node  
{ 
  struct Request data; 
  struct node *next; 
};


void dosomething();

//array of accounts
struct account *allaccs;
//number of workers required
int Numworker;
//number of accounts
int numacc;
//name of the output file
char* outputfile;

//for the requests list
pthread_mutex_t linkedlist;

//for putting our output there
FILE *thefile;

struct node *root;
struct node *last;

int main(int argc, char * argv[])
{

//Get the user Input from command promot on server setup 
if(argv[1]!=0&&argv[2]!=0&&argv[3]!=0){
    Numworker= atoi(argv[1]);
    numacc= atoi(argv[2]);
    outputfile = argv[3];
    thefile= (char *) malloc(sizeof(char *) * 128);
    thefile = fopen(outputfile, "w");
}
  
  //Make the bank accounts
  initialize_accounts(numacc);
  
  //make a mutex for each account
  int i=0;
  allaccs = (int *) malloc((sizeof(pthread_mutex_t)+sizeof(int)) * numacc);
  
  for(i=1;i<numacc;i++)
  {
    struct account acc;
    pthread_mutex_init(&acc.lock, NULL);
    acc.value=read_account(i);
    allaccs[i]=acc;
   } 
   
   //all the worker threads
  pthread_t workerth[Numworker];
  
  //make the worker threads
  for(i = 0; i < Numworker; i++)
  {
        // Create new thread
       pthread_create(&workerth[i], 0, dosomething, NULL);
  }
  
  int request_id=0;
  
  root = NULL;
  last = NULL;
  root = last;
  pthread_mutex_init(&linkedlist, NULL);
  while(1){
   
    //Parse and read from terminal
    char command[1024];
    char *word[1024];
    //get input from command line
    fgets(command, sizeof(command), stdin);

    //store the words in the line inside an array
    int i=0;
    char* token = strtok(command, " "); 
    word[0]=token;
    if(strcmp("END\n", word[0])==0){
      exit(0);
    }
    while (token != NULL) { 
    i++;
         token= strtok(NULL, " "); 
         word[i]=token;
    }
    
    //-----fgets is weird it adds \n on the last input so this is to remove that
    int thelength = strlen(word[i-1]);
    char *booga= word[i-1];
    booga[thelength-1] = 0;
    word[i-1]=booga;
    //-----fgets is weird it adds \n on the last input
    struct Request arequest;
    gettimeofday(&arequest.time, NULL);
     memcpy(&arequest.whattodo, &word,sizeof word);
     arequest.request_id=request_id++;
     
      pthread_mutex_lock(&linkedlist);
      if(root == NULL)
        {
            
            root = (struct node *) malloc(sizeof(struct node*));
            root->next = NULL;
            root->data=arequest;
            last = root;
        }
        else if(last->next == NULL)
        {  
            struct node *new;
            new = (struct node *) malloc(sizeof(struct node*));
            new->data=arequest;
            new->next = NULL;
            last->next = new;
            last = new;
        }
         pthread_mutex_unlock(&linkedlist);
         fclose(thefile);
  }
}


void transacation(int id,char* whattodo[1024],  struct timeval time_start ){
    int accounts[10] = {0};
    int amounts[10] = {0};
    int i=0;
    int numtransc=0;
    int enoughbalance = 0;
    //take note of the accounts to transfer and the amounts
    while(whattodo[i]!=NULL){
      accounts[i]=atoi(whattodo[i]);
      i++;
      amounts[i]=atoi(whattodo[i]);
      numtransc++;
    }

    for (i = 1; i < numtransc+1; i++)
    {
        
        pthread_mutex_lock(&allaccs[accounts[i]].lock);
    }

     // checking for the balance 
    for (i = 1; i < numtransc+1; i++)
    { 
        int value = read_account(accounts[i]);
        if(value + amounts[i] < 0)
        {
        struct timeval end;
        gettimeofday(&end, NULL);
        fprintf(thefile, "%d ISF %d TIME %d.%06d %d.%06d\n", id, accounts[i], time_start.tv_sec, time_start.tv_usec, end.tv_sec, end.tv_usec);
         enoughbalance=1;
        } 
    }

    if(enoughbalance==0){
    //do the transfer
     for (i = 1; i < numtransc+1; i++)
        {
            int balance = read_account(accounts[i]);
            write_account(accounts[i], balance + amounts[i]);
        }
        struct timeval end;
        gettimeofday(&end, NULL);
        fprintf(thefile, "%d OK TIME %d.%06d %d.%06d\n", id, time_start.tv_sec, time_start.tv_usec, end.tv_sec, end.tv_usec);
    }
    for (i = 1; i < numtransc+1; i++)
    {
        pthread_mutex_unlock(&allaccs[accounts[i]].lock);
    }
}


//to check balance
void balance(int requestid, int accid, struct timeval time_start)
{
    pthread_mutex_lock(&allaccs[accid].lock);
    int balance = read_account(accid);
    pthread_mutex_unlock(&allaccs[accid].lock);
    struct timeval end;
    gettimeofday(&end, NULL);

    fprintf(thefile, "%d BAL %d TIME %d.%06d %d.%06d\n", requestid, balance, time_start.tv_sec, time_start.tv_usec, end.tv_sec, end.tv_usec);
}


void dosomething(){
  while(root != NULL){
    pthread_mutex_lock(&linkedlist);
    struct Request therequest;
    therequest= root->data;
    root= root ->next;
    pthread_mutex_unlock(&linkedlist);
    //now we handle the request that we got from the list
    printf("ID %d\n", therequest.request_id);
    char *commands[1024];
    memcpy(&commands, &therequest.whattodo,sizeof commands);
    if(strcmp("TRANS",commands[0])==0){
      //handle transacations
      transacation(therequest.request_id,commands, therequest.time );
    }
    if(strcmp("CHECK",commands[0])==0){
      //handle balance
      balance(therequest.request_id,atoi(commands[1]), therequest.time);
    } 
  }
}


