#include "queue.h"
#include <stdio.h>

void init_queue(queue *q)
{
        q->first = 0;
        q->last = QUEUESIZE-1;
        q->count = 0;
}

void enqueue(queue *q, queue_item x)
{
        if (q->count >= QUEUESIZE)
		printf("Warning: queue overflow enqueue\n");
        else {
                q->last = (q->last+1) % QUEUESIZE;
                q->q[ q->last ] = x;    
                q->count = q->count + 1;
        }
}

queue_item dequeue(queue *q)
{
        queue_item x;

        if (q->count <= 0) printf("Warning: empty queue dequeue.\n");
        else {
                x = q->q[ q->first ];
                q->first = (q->first+1) % QUEUESIZE;
                q->count = q->count - 1;
        }

        return x;
}

void print_queue(queue *q)
{
        int i;

        i=q->first; 
        
        while (i != q->last) {
                printf("%s ",q->q[i].s);
                i = (i+1) % QUEUESIZE;
        }

        printf("%s ",q->q[i].s);
        printf("\n");
}
