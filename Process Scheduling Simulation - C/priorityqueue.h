#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
A queue of data with a specified maximum capacity.  Items entering the queue are compared with the cmp function.
*/
typedef struct 
{
	size_t pq_size; //Current number of items in the queue
	size_t pq_capacity;
	void ** pq_data; //The actual queue of data
	int (* cmp)(const void * arg1, const void * arg2); //Design compare functions according to this
} priority_queue;

/**
Creates a new priority queue.
int ( *cmp ) ( const void* element1, const void* element2 ) : A pointer to a function designed as such to compare the elements entering the queue.
size_t capacity : The maximum size of this priority queue.
*/
priority_queue* pq_create( int ( *cmp ) ( const void* element1, const void* element2 ), size_t capacity )
{
	priority_queue * newpq = malloc(sizeof(priority_queue));
	newpq -> pq_size = 0;
	newpq -> pq_capacity = capacity;
	newpq -> cmp = cmp;
	newpq -> pq_data = calloc(capacity, sizeof(*(newpq->pq_data)));
	return newpq;
}

/**
Frees the memory held by the specified priority queue.
priority_queue* pq : The priority queue to destroy
*/
void pq_destroy( priority_queue* pq )
{
	free(pq -> pq_data);
	free(pq);
}

/**
Determines whether the specified priority queue is empty (1 if true).
priority_queue* pq : The priority queue to check
*/
int pq_empty( priority_queue* pq ) 
{
	if(pq-> pq_size == 0)
	{
		return 1;
	}
	return 0;
}

/**
Inserts an item into the specified priority queue
priority_queue* pq : The queue to qdd the item to
const void* element : The item to add
*/
void pq_insert( priority_queue* pq, const void* element )
{
	if (pq == NULL) //If the specified queue does not exist
	{
		return;
	}
	if((pq -> pq_size) >= (pq -> pq_capacity)) //If the queue is at capacity
	{
		printf("ERROR: Queue has already reached capacity of %d\n", (pq -> pq_capacity));
		return;
	}
	if((pq -> pq_size) == 0) //If the queue is currently empty
	{
		pq -> pq_data[0] = (void * )element;
		pq -> pq_size ++;
		//printf("Empty, Inserted at position 0\n");
		return;
	}
	void * tmp = NULL;
	size_t pos = (pq -> pq_size);
	pq -> pq_data[pq -> pq_size] = (void * )element;
	pq -> pq_size++;
	while(pos > 0 && pq -> cmp(pq -> pq_data[pos], pq -> pq_data[pos - 1]) > 0)
	{ //While not at the head of the queue, and the item at position is greater than the item before it
		tmp = pq -> pq_data[pos];
		pq -> pq_data[pos] = pq -> pq_data[pos - 1];
		pq -> pq_data[pos - 1] = tmp;
		pos--;
	}
	//printf("Inserted at position %d\n", pos);
}

/**
Returns the maximum item in the queue (at position 0)
priority_queue* pq : The priority queue to use
*/
void* pq_max( priority_queue* pq )
{
	if(pq == NULL || (pq -> pq_size <= 0)) //If pq does not exist
	{
		return NULL;
	}
	return (pq -> pq_data[0]);
}

/**
Removes and returns the maximum item in the queue (at position 0)
priority_queue* pq : The priority queue to use
*/
void* pq_remove_max( priority_queue* pq )
{
	if(pq == NULL || (pq -> pq_size <= 0))//If pq does not exist
	{
		return NULL;
	}
	void * retval = pq -> pq_data[0];
	if(pq -> pq_size == 1)//If there is only one item, return it and set size to 0
	{
		pq -> pq_size = 0;
		return retval;
	}
	size_t pos = 0;
	while(pos < pq -> pq_size) //While there is another item to move
	{
		pq -> pq_data[pos] = pq -> pq_data[pos + 1]; //Move to the left
		pos++;
	}
	pq -> pq_size--;
	return retval;
}



#endif