#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include "code.h"
#include "queue.h"


int size;
char *s, *r, *se, *sd;

char * getXOR(char * s1, char *s2, int size) {
  int i;
  char * tmp = malloc(size * sizeof(char));

  for (i = 0; i < size -1; i++)
    *(tmp+i) = s1[i] ^ s2[i];

  return tmp;
}

void *twFunction(void * ptr) {
  printf("SD: %s\n", sd);
}

void *tdFunction (void * ptr) {
  sd = getXOR(r, se, size);
}

void *teFunction (void * ptr) {
  int i, rfd;
  char c;

  r = malloc(size * sizeof(char));

  rfd = open("/dev/random", O_RDONLY);

  for (i = 0; i < size - 1; i++) {
    read(rfd, &c, sizeof(char));
    *(r+i) = abs(c) % 26 + 65;
  }

  close(rfd);

  se = getXOR(s, r, size);

  printf("R: %s\n", r);
  printf("SE: %s\n", se);
}

void *trFunction (void * ptr) {
  s = (char *) malloc (N_BYTES + 1);

  printf("$> ");
  size = getline(&s, &N_BYTES, stdin);

  return NULL;
}

int main() {
  pthread_t tr, te, td, tw;

  queue q;
  init_queue(&q);
  queue_item qi;
  qi.s = "ciao";
  qi.size = 5;
  enqueue(&q, &qi);

  queue_item qi1;
  qi1.s = "bello";
  qi1.size = 5;
  enqueue(&q, &qi1);

  print_queue(&q);
/*
  pthread_create(&tr, NULL, &trFunction, NULL);
  pthread_join(tr, NULL);

  pthread_create(&te, NULL, &teFunction, NULL);
  pthread_join(te, NULL);

  pthread_create(&td, NULL, &tdFunction, NULL);
  pthread_join(td, NULL);

  pthread_create(&tw, NULL, &twFunction, NULL);
  pthread_join(tw, NULL);
*/
  return 0;
}
