#include <stdlib.h>
#include <stdio.h>
#include "priorityqueue.h"

#define QUANTUM 10

/**
A struct that simulates a process for the purpose of scheduling algorithms.
*/
typedef struct
{
	int id;
	int priority;
	int arrtime;
	int burst;
}process;


/**
Compares two processes by their priority, returning a positive value if the first priority is lesser, 0 if the priority is equal, and a negative value if the second priority is lesser.
const void * element1 : The first priority to compare
const void * element2 : The second priority to compare
*/
int priority_cmp(const void * element1, const void * element2)
{
	process * p1 = (process*)element1;
	process * p2 = (process*)element2;
	return p2 -> priority - p1 -> priority;
}

/**
Compares two processes by their burst, returning a positive value if the first burst is lesser, 0 if the burst is equal, and a negative value if the second burst is lesser.
const void * element1 : The first burst to compare
const void * element2 : The second burst to compare
*/
int burst_cmp(const void * element1, const void * element2)
{
	process * p1 = (process*)element1;
	process * p2 = (process*)element2;
	return p2 -> burst - p1 -> burst;
}

/**
Simulates a 'First Come, First Served' algorithm.  Processes are run in the order that they arrive at the scheduler.
process p[] : An array of processes to run the algorithm on
int length : The length of the process array
*/
void scheduleFCFS(process p[], int length)
{
	int timer = 0; //Total time that the algorithm runs
	int wtime = 0; //Total time processes wait before being initialized
	int ttime = 0; //Time between when a process is activated and when it finishes.

	printf("Running FCFS algorithm\n");
	int i;
	for(i = 0; i < length; i++)
	{
		//printf("Running process %d\n", p[i].id);
		int preptime = 0;
		if(timer < p[i].arrtime)
		{
			preptime = p[i].arrtime - timer;
			//usleep(1000 * preptime);
			timer = p[i].arrtime;
		}
		//printf("Started at %d ms ", timer);
		wtime += timer;
		ttime += (p[i].burst + preptime);
		timer += p[i].burst;
		//usleep(1000 * p[i].burst);
		//printf("Finished at %d ms\n", timer);
	}
	printf("Total time: %d ms Avg wait: %d ms Avg turnaround: %d ms Processes/s: %1.4f\n", timer, (int)(wtime / length), (int)(ttime / length),  ((float)length / ((float)timer / 1000)));
	
}

/**
Simulates a 'Shortest Job First' algorithm.  The scheduler arranges the processes by their burst time, and runs them in order from least to greatest.
process p[] : An array of processes to run the algorithm on
int length : The length of the process array
*/
void scheduleSJF(process p[], int length)
{
	int timer = 0;
	int wtime = 0;
	int ttime = 0;

	printf("Running SJF algorithm\n");

	priority_queue * pq;
	pq = pq_create(burst_cmp, length);

	int i;
	for(i = 0 ; i < length; i++)
	{
		pq_insert(pq, &p[i]);
	}	
	
	while(!pq_empty(pq))
	{
		//printf("Running process %d\n", ((process*)pq_max(pq)) -> id);
		int preptime = 0;
		if(timer < ((process*)pq_max(pq)) -> arrtime)
		{
			preptime = ((process*)pq_max(pq)) -> arrtime - timer;
			//usleep(1000 * preptime);
			timer += ((process*)pq_max(pq)) -> arrtime;
		}
		//printf("Started at %d ms ", timer);
		wtime += timer;
		ttime += (((process*)pq_max(pq)) -> burst + preptime);
		timer += ((process*)pq_remove_max(pq)) -> burst;
		//usleep(1000 * p[i].burst);
		//printf("Finished at %d ms\n", timer);
	}
	printf("Total time: %d ms Avg wait: %d ms Avg turnaround: %d ms Processes/s: %1.4f\n", timer, (int)(wtime / length), (int)(ttime / length),  ((float)length / ((float)timer / 1000)));
}

/**
Simulates a Non-Preemptive Priority algorithm.  The scheduler arranges the processes by their priority, and runs them in order, with the smallest priority being run first.
process p[] : An array of processes to run the algorithm on
int length : The length of the process array
*/
void scheduleNPP(process p[], int length)
{
	int timer = 0;
	int wtime = 0;
	int ttime = 0;

	printf("Running NPP algorithm\n");

	priority_queue * pq;
	pq = pq_create(priority_cmp, length);

	int i;
	for(i = 0 ; i < length; i++)
	{
		pq_insert(pq, &p[i]);
	}	
	
	while(!pq_empty(pq))
	{
		//printf("Running process %d\n", ((process*)pq_max(pq)) -> id);
		int preptime = 0;
		if(timer < ((process*)pq_max(pq)) -> arrtime)
		{
			preptime = ((process*)pq_max(pq)) -> arrtime - timer;
			//usleep(1000 * preptime);
			timer += ((process*)pq_max(pq)) -> arrtime;
		}
		//printf("Started at %d ms ", timer);
		wtime += timer;
		ttime += (((process*)pq_max(pq)) -> burst + preptime);
		timer += ((process*)pq_remove_max(pq)) -> burst;
		//usleep(1000 * p[i].burst);
		//printf("Finished at %d ms\n", timer);
	}
	printf("Total time: %d ms Avg wait: %d ms Avg turnaround: %d ms Processes/s: %1.4f\n", timer, (int)(wtime / length), (int)(ttime / length),  ((float)length / ((float)timer / 1000)));
}

/**
Simulates a 'Round Robin" algorithm.  Each of the processes is given a certain quantum, or length of time, to run.  The scheduler loops through the processes.  Once a process has finished, it is removed from the loop.  The algorithm ends when all processes have finished.
process p[] : An array of processes to run the algorithm on
int length : The length of the process array
*/
void scheduleRR(process p[], int length)
{
	int * remtimes = malloc(length * sizeof(int));

	int j;
	for(j = 0; j < length; j++)
	{
		remtimes[j] = p[j].burst;
	}

	int hasRemaining = 1;

	int timer = 0;
	int wtime = 0;
	int ttime = 0;

	printf("Running Round Robin algorithm\n");

	while(hasRemaining)
	{
		hasRemaining = 0;
		int i;
		for(i = 0; i < length; i++)
		{
			if(remtimes[i] <= 0)
			{
				//printf("Process %d has already finished.\n", p[i].id);
			}
			else
			{
				hasRemaining = 1;
				//printf("Running process %d\n", p[i].id);
				int preptime = 0;
				if(timer < p[i].arrtime)
				{
					preptime = p[i].arrtime - timer;
					usleep(preptime);
					timer = p[i].arrtime;
					wtime += timer;
				}
				remtimes[i] -= QUANTUM;
				ttime += (QUANTUM + preptime);
				timer += QUANTUM;
				//usleep(1000 * QUANTUM);
				//printf("Process %d has %d ms remaining\n", p[i].id, remtimes[i]);
				if(remtimes[i] <= 0)
				{
					//printf("Process %d finished at %d ms\n", p[i].id, timer);
				}
			
			}
		
		}
	}
	printf("Total time: %d ms Avg wait: %d ms Avg turnaround: %d ms Processes/s: %1.4f\n", timer, (int)(wtime / length), (int)(ttime / length),  ((float)length / ((float)timer / 1000)));
}




int main(int argn, char * args[])
{
	
	FILE * fp;
	fp = fopen("processes.txt", "r");
	if(!fp)
	{
		printf("Could not open file\n");
		return -1;
	}
	int length;
	fscanf(fp, "%d", &length);
	process * plist = malloc(sizeof(process)*length);

	int counter = 0;
	while (fscanf(fp, "%d %d %d %d", &plist[counter].id, &plist[counter].priority, &plist[counter].arrtime, &plist[counter].burst) != EOF && counter < length)
	{
		printf("%d %d %d %d\n", plist[counter].id, plist[counter].priority, plist[counter].arrtime, plist[counter].burst);
		++counter;
	}

	scheduleFCFS(plist, length);

	scheduleSJF(plist, length);

	scheduleNPP(plist, length);

	scheduleRR(plist, length);

	
	return 0;
}

