#include "queue.h"
#include <stdio.h>

void init_queue(queue *q)
{
/* Initialize the queue */
  q->first = 0;
  q->last = QUEUESIZE-1;
  q->count = 0;
}

void enqueue(queue *q, queue_item *qi)
{
/* Add a queue's item to the queue */

  q->last = (q->last+1) % QUEUESIZE;
  q->qi[ q->last ] = *qi;
  q->count = q->count + 1;
}

queue_item dequeue(queue *q)
{
/* Remove a queue's item from the queue */

  queue_item x;

  if (q->count <= 0) printf("Warning: empty queue dequeue.\n");
  else {
    x = q->qi[ q->first ];
    q->first = (q->first+1) % QUEUESIZE;
    q->count = q->count - 1;
  }

  return x;
}

int is_empty(queue *q) {
/* Check if the queue is empty */
  return q->count == 0;
}

void print_queue(queue *q)
{
/* For debug, print all the queue */

  int i;

  i=q->first;

  while (i != q->last) {
    printf("%s ",q->qi[i].s);
    i = (i+1) % QUEUESIZE;
  }

  printf("%s ",q->qi[i].s);
  printf("\n");
}
