#define QUEUESIZE       1000

typedef struct {
  char * s;
  int size;
} queue_item;

typedef struct {
  queue_item q[QUEUESIZE];		/* body of queue */
  int first;                      /* position of first element */
  int last;                       /* position of last element */
  int count;                      /* number of queue elements */
} queue;


void enqueue(queue *q, queue_item *qi);
void init_queue(queue *q);
void print_queue(queue *q);
queue_item dequeue(queue *q);
