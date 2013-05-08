#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include "code.h"
#include "queue.h"
#include "log.h"
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

int size;
char *r, *se, *sd;
sem_t tr2te;
queue q;

char * getXOR(char * s1, char *s2, int size) {
  int i;
  char * tmp = malloc(size * sizeof(char));

  for (i = 0; i < size -1; i++)
    *(tmp+i) = s1[i] ^ s2[i];

  return tmp;
}

void *twFunction(void * ptr) {
  printf("SD: %s\n", sd);
  return NULL;
}

void *tdFunction (void * ptr) {
  sd = getXOR(r, se, size);
  return NULL;
}

void *teFunction (void * ptr) {
  int i, rfd;
  char c;
  char * s = NULL;

  printf("%d", &tr2te);

  sem_wait(&tr2te);

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

  return NULL;
}

void *trFunction (void * ptr) {
  queue_item qi;
  char * s = (char *) malloc (N_BYTES + 1);
  char * end = "quit";

  do {
    printf("$> ");
    size = getline(&s, &N_BYTES, stdin);
    s[strlen(s)-1] = '\0';  // remove last char of this string

    qi.s = s;
    qi.size = size;
    enqueue(&q, &qi);

    Log(s, "tr.log");
    sem_post(&tr2te);
  } while (strcmp(s, end) != 0);

  return NULL;
}

int main() {
  pthread_t tr, te, td, tw;

  if (sem_init(&tr2te, 0,1)) {
    printf("init");
  }

//  sem_init(&tr2te, 0, 0);  // TODO[ml] catch errors
/*
  init_queue(&q);

  pthread_create(&tr, NULL, &trFunction, NULL);
  pthread_create(&te, NULL, &teFunction, NULL);

  pthread_join(tr, NULL);
  pthread_join(te, NULL);

  pthread_create(&td, NULL, &tdFunction, NULL);
  pthread_join(td, NULL);

  pthread_create(&tw, NULL, &twFunction, NULL);
  pthread_join(tw, NULL);
*/
  return 0;
}
