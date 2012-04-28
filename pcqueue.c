#include "pcqueue.h"
#include "stdlib.h"
#include "stdio.h"
#include <string.h>
/* 
 * allocate a new producer-consumer work queue
 * @max_limit - limit on maximum size of the queue
 */
struct work_queue* alloc_queue(int max_limit, char* name)
{
	struct work_queue* wq = (struct work_queue*)malloc(sizeof(struct work_queue));
	wq->head = wq->tail = NULL;
	wq->size = 0;
	wq->qname = (char*)malloc((sizeof(name)+1)*sizeof(char));
	strcpy(wq->qname, name);
	wq->max_size = max_limit;
	pthread_mutex_init(&wq->queue_lock, NULL);
	return wq;
}

/*
 * Get the current size of the queue
 * @wq - pointer to the work-queue instance 
 */
int get_queue_size(struct work_queue* wq)
{
	return wq->size; /* Other threads might be concurrently updating size . Is it fine? */
}

/*
 * Get the capacity of the queue
 * @wq - pointer to the work-queue instance
 */
int get_queue_capacity(struct work_queue* wq)
{
        return wq->max_size;
}

/*
 * Add a generic job to the bottom of queue [production]( could be of any type (void*) )
 * That means this can even be a heterogeneous queue for practical purposes 
 * @wq - pointer to a work-queue instance
 * @job - Job to be pused to the queue
 * @return - negative(failure), zero(success)
 */
int add_job_to_queue(struct work_queue* wq,void* job)
{
	struct work_queue_node* next_job = (struct work_queue_node*)malloc(sizeof(struct work_queue_node));
	int add_success = 0, try_count = 0;
	
	next_job->next = NULL;
	next_job->data = job;
	
	TRY_ADDING_TO_QUEUE:
	pthread_mutex_lock(&wq->queue_lock);	
	if(wq->size < wq->max_size)
	{
		/* If both head and tail are NULL before this insertion */
		if(NULL==wq->tail)
			wq->head = wq->tail = next_job;
		else {
			/* Inserting into end of queue */
			wq->tail->next = next_job;
			wq->tail = wq->tail->next;
		}
		wq->size++;
		add_success = 1;
	}
	try_count++;
	pthread_mutex_unlock(&wq->queue_lock);
		
	if(!add_success)
		{
		if(try_count > 15) sleep(3); /* Sleep for a while and the queue might get freed-up*/
		goto TRY_ADDING_TO_QUEUE;		
		}		
	printf("PCQUEUE : Added element to %s (size:%d)\n", wq->qname, wq->size);
	return 0;
}

/*
 * Get a job from the top of the work queue [consumption]
 * @wq - pointer to the 
 */
void* get_job_from_queue(struct work_queue* wq)
{
	struct work_queue_node* next_job_node = NULL;
	void* job = NULL;	

	pthread_mutex_lock(&wq->queue_lock);
	next_job_node = wq->head;
	if(wq->head == wq->tail)
	{
		wq->head = wq->tail = NULL;
	}
	else if(wq->head != NULL){
		wq->head = wq->head->next;
	}
	wq->size--;
	pthread_mutex_unlock(&wq->queue_lock);
	
	if(next_job_node != NULL)
		{	
		job = next_job_node->data;
		free(next_job_node);
		}
	
	printf("PCQUEUE : Removed element from %s (size:%d)\n", wq->qname, wq->size);

	return job;
}
