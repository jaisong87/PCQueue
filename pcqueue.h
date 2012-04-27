#ifndef __PCQUEUE_H__
#define __PCQUEUE_H__
#include<pthread.h>

/* Internal node for generic work_queue */
struct work_queue_node{
void* data;
struct work_queue_node *next;
};

/* generic work_queue */
struct work_queue{
struct work_queue_node *head,*tail;
int size, max_size;
pthread_mutex_t queue_lock;
};

/* work-queue API functions */
struct work_queue* alloc_queue(int); /* allocate a new work-queue */
int add_job_to_queue(struct work_queue*,void*); /* Insert a new job */
void* get_job_from_queue(struct work_queue*);
#endif
