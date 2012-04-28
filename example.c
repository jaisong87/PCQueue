#include "pcqueue.h"
#include<stdio.h>
#include<stdlib.h>

void push_to_queue(struct work_queue* wq, int i)
{
int *data = (int*)malloc(sizeof(int));
*data = i;
add_job_to_queue((struct work_queue*)wq, (void*)data); 
}

int* pop(struct work_queue* wq)
{
int *data =(int*) get_job_from_queue(wq);
return data;
}

int main()
{
int N=100, i;
int *data;
struct work_queue* wq = alloc_queue(1000, "Num queue");

for(i=0;i<N;i++)
	{
		push_to_queue(wq,i);	
	}
printf("Done pushing %d elements\n", N);
printf("Size of queue is %d elements\n", get_queue_size(wq));

i=0;
while(data = pop(wq))
	{
	printf("%d\n",*data);
	i++;
	}

printf("Popped out %d elements\n", i);
printf("Size of queue is %d elements\n", get_queue_size(wq));
return 0;
}
