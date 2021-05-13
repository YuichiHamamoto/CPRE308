/*******************************************************************************
 * *
 * * CprE 308 Scheduling Lab
 * *
 * * scheduling.c
 * * last updated 2020-03-01 - mlw
 * *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_PROCESSES 20
#define NUM_SCHEDULERS 4

#define SCHEDULER_NAME_LEN 30

/* Defines a simulated process */
typedef struct process {
  int arrivaltime;    /* Time process arrives and wishes to start */
  int starttime;      /* Time process actually starts */
  int runtime;        /* Time process requires to complete job */
  int remainingtime;  /* Runtime remaining before process completes */
  int endtime;        /* Time process finishes */
  
  int priority;       /* Priority of the process */

  int running;        /* Convenience - indicates if the process is currently running */
  int finished;       /* Convenience - indicates if the process has finished */
} process;

/* Defines a simulated scheduler */
typedef struct scheduler
{
  int (* func) (const process [], int); /* Function to run for the scheduler */
  char name[SCHEDULER_NAME_LEN];  /* Name of the scheduler, for pretty printing */
} scheduler;

/* Prototypes of scheduler functions */
int first_come_first_served(const process proc[], int t);
int shortest_remaining_time(const process proc[], int t);
int round_robin(const process proc[], int t);
int round_robin_priority(const process proc[], int t);

/* Main, contains most of the simulation code */
int main() {
  int i,j;
  process proc[NUM_PROCESSES],        /* List of processes */
            proc_copy[NUM_PROCESSES]; /* Backup copy of processes */
            
  /* Initialize the schedulers */
  scheduler schedulers[NUM_SCHEDULERS];
  schedulers[0].func = first_come_first_served;
  strncpy(schedulers[0].name, "First come first served", SCHEDULER_NAME_LEN);
  schedulers[1].func = shortest_remaining_time;
  strncpy(schedulers[1].name, "Shortest remaining time", SCHEDULER_NAME_LEN);
  schedulers[2].func = round_robin;
  strncpy(schedulers[2].name, "Round robin", SCHEDULER_NAME_LEN);
  schedulers[3].func = round_robin_priority;
  strncpy(schedulers[3].name, "Round robin with priority", SCHEDULER_NAME_LEN);

  /* Seed random number generator */
  //srand(time(0));    /* Use this seed to test different inputs */
  srand(0xC0FFEE);     /* Use this seed to test fixed input */

  /* Initialize process structures */
  for(i=0; i<NUM_PROCESSES; i++)
  {
    proc[i].arrivaltime = rand() % 100;
    proc[i].runtime = (rand() % 30) + 10;
    proc[i].remainingtime = proc[i].runtime;
    proc[i].priority = rand() % 3;
	
    proc[i].starttime = -1;
    proc[i].endtime = -1;
    proc[i].running = 0;
    
    proc[i].finished = 0;
  }

  // //TEST CASE
  // proc[0].arrivaltime = 10;
  // proc[0].runtime = 25;
  // proc[0].priority = 0;

  // proc[1].arrivaltime = 69;
  // proc[1].runtime = 36;
  // proc[1].priority = 2;

  // proc[2].arrivaltime = 87;
  // proc[2].runtime = 20;
  // proc[2].priority = 0;

  // proc[3].arrivaltime = 1;
  // proc[3].runtime = 16;
  // proc[3].priority = 2;

  // proc[4].arrivaltime = 46;
  // proc[4].runtime = 28;
  // proc[4].priority = 0;

  // proc[5].arrivaltime = 92;
  // proc[5].runtime = 14;
  // proc[5].priority = 1;

  // proc[6].arrivaltime = 74;
  // proc[6].runtime = 12;
  // proc[6].priority = 1;

  // proc[7].arrivaltime = 61;
  // proc[7].runtime = 28;
  // proc[7].priority = 0;

  // proc[8].arrivaltime = 89;
  // proc[8].runtime = 27;
  // proc[8].priority = 0;

  // proc[9].arrivaltime = 28;
  // proc[9].runtime = 31;
  // proc[9].priority = 1;

  // proc[10].arrivaltime = 34;
  // proc[10].runtime = 33;
  // proc[10].priority = 2;

  // proc[11].arrivaltime = 82;
  // proc[11].runtime = 13;
  // proc[11].priority = 1;

  // proc[12].arrivaltime = 93;
  // proc[12].runtime = 32;
  // proc[12].priority = 0;

  // proc[13].arrivaltime = 85;
  // proc[13].runtime = 33;
  // proc[13].priority = 0;

  // proc[14].arrivaltime = 87;
  // proc[14].runtime = 11;
  // proc[14].priority = 1;

  // proc[15].arrivaltime = 57;
  // proc[15].runtime = 35;
  // proc[15].priority = 1;

  // proc[16].arrivaltime = 2;
  // proc[16].runtime = 10;
  // proc[16].priority = 0;

  // proc[17].arrivaltime = 27;
  // proc[17].runtime = 31;
  // proc[17].priority = 0;

  // proc[18].arrivaltime = 34;
  // proc[18].runtime = 10;
  // proc[18].priority = 0;

  // proc[19].arrivaltime = 78;
  // proc[19].runtime = 18;
  // proc[19].priority = 1;

  // for(i=0; i<NUM_PROCESSES; i++)
  // {
  //   proc[i].remainingtime = proc[i].runtime;
	
  //   proc[i].starttime = -1;
  //   proc[i].endtime = -1;
  //   proc[i].running = 0;
    
  //   proc[i].finished = 0;
  // }

  /* Print process values */
  printf("Process\tarrival\truntime\tpriority\n");
  for(i = 0; i < NUM_PROCESSES; i++) {
    printf("%d\t%d\t%d\t%d\n", i, proc[i].arrivaltime, proc[i].runtime,
           proc[i].priority);
  }
  
  /* Run simulation for each scheduler */
  for(i = 0; i < NUM_SCHEDULERS; i++) {
    int num_finished = 0;
    int current_time = 0;
    int prev_pid = -1;
    int total_turnaround_time = 0;
    printf("\n\n--- %s\n", schedulers[i].name);
    
    /* Use a copy of the processes array */
    memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(process));
    
    /* Run the simulation until all processes finish */
    while(num_finished < NUM_PROCESSES) {
      process * p;
      
      /* Call the scheduler */
      int next_pid = schedulers[i].func(proc_copy, current_time);
      
      /* If there's no process to run, just move time forward */
      if(next_pid < 0) {
        current_time += 1;
        continue;
      }
      
      /* Convenience - use p to reference the proc */
      p = &proc_copy[next_pid];
      
      /* If the process just started, print a message */
      if(p->starttime == -1) {
        printf("%03d: Process %d started\n", current_time, next_pid);
        p->starttime = current_time;
      }
      
      /* Update which process is running */
      if(prev_pid >= 0) {
        proc_copy[prev_pid].running = 0;
      }
      prev_pid = next_pid;
      p->running = 1;
      
      /* Move time forward */
      current_time += 1;
      
      /* Update remaining time of the process that just ran */
      p->remainingtime -= 1;
      
      /* If the process already finished, it shouldn't have been chosen */
      if(p->remainingtime < 0) {
        printf("Error: tried to run finished process %d\n", next_pid);
        continue;
      /* If the process just finished, print a message and do bookkeeping */
      } else if(p->remainingtime == 0) {
        printf("%03d: Process %d finished\n", current_time, next_pid);
        p->endtime = current_time;
        p->finished = 1;
        num_finished += 1;
        /* Keep a running total of turnaround time */
        total_turnaround_time += p->endtime - p->arrivaltime;
      }
    }
    
    /* Print average turnaround time */
    printf("Average turnaround time: %.01lf seconds\n", (double)total_turnaround_time / NUM_PROCESSES);
  }

  return 0;
}

/* ----------------------Schedulers------------------------------*/
/* Each scheduler function should return the index of the process 
   from proc[] that should run next. Parameter t is the current time 
   of the simulation. Note that proc[] contains all processes, 
   including those that have not yet arrived and those that are finished.
   You are responsible for ensuring that you schedule a "ready"
   process. If no processes are ready, return -1.
*/
   
int first_come_first_served(const process proc[], int t)
{
  //counter
	int i;

  //find frist come
  int fcome=-1;
  for(i = 0; i < NUM_PROCESSES; i++){
    if(proc[i].arrivaltime<=t&&!proc[i].finished){
      if(fcome<0){
        fcome = i;
      }else if(proc[i].arrivaltime<proc[fcome].arrivaltime){
        fcome = i;
      }
    }
  }
  return fcome;
}

int shortest_remaining_time(const process proc[], int t)
{
  //counter
	int i;

	// shortest remaining time
	int stime=-1;

	/*loop through num processes to execute them*/
	for(i = 0; i < NUM_PROCESSES; i++)
	{
		/*initialize shortest_remaining_time*/
		
			//if shortest_remaining time hasn't been set
			if(proc[i].arrivaltime <= t && !proc[i].finished)
			{
        if(stime < 0){
          stime = i;
        }
        else if(proc[i].remainingtime < proc[stime].remainingtime){
          stime = i;
        }
				
			}
	}
  return stime;
}

int round_robin(const process proc[], int t)
{
  //counter
	int i;
	// shortest remaining time
	int robin=-1;

  static double time [20];

  for(i = 0; i < NUM_PROCESSES; i++){   
    if(!proc[i].finished&&proc[i].arrivaltime<=t){
      if(proc[i].arrivaltime==t){
        time[i]=t;
      }
      if(robin<0){
        robin = i;
      }
      else if(time[i]<time[robin]){
        robin = i;
      }
    }
  } 
  time[robin]=t+1.1;
  return robin;
}

int round_robin_priority(const process proc[], int t)
{
  //counter
	int i;
	// shortest remaining time
	int robin=-1;

  //Store the time addded in the que
  static double time [20];

  for(i = 0; i < NUM_PROCESSES; i++){   
    if(!proc[i].finished&&proc[i].arrivaltime<=t){
      if(proc[i].arrivaltime==t){
        time[i]=t;
      }
      if(robin<0){
        robin = i;
      }
      if(proc[i].priority>proc[robin].priority){
        robin=i;
      }
      else if(proc[i].priority==proc[robin].priority){
        if(time[i]<time[robin]){
                  robin = i;
        }
      }
    }
  } 
  time[robin]=t+1.1;
  return robin;
}
