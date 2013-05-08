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
sem_t * tr2te;
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
  queue_item qi;

  if(is_empty(&q)) {
    sem_wait(tr2te);
  }

  qi = dequeue(&q);
  printf("%s\n", qi.s);

  r = malloc(size * sizeof(char));

  printf("canarino\n");
  rfd = open("/dev/random", O_RDONLY);

  printf("pesce rosso\n");
  for (i = 0; i < size - 1; i++) {
    read(rfd, &c, sizeof(char));
    *(r+i) = abs(c) % 26 + 65;
  }

  close(rfd);

  se = getXOR(qi.s, r, size);

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
    printf("cucuLADRO\n");
    s[strlen(s)-1] = '\0';  // remove last char of this string

    printf("cucuLADRO\n");
    qi.s = s;
    qi.size = size;
    enqueue(&q, &qi);

    Log(s, "tr.log");
    printf("cucu\n");
    sem_post(tr2te);
    printf("cucu2\n");
  } while (strcmp(s, end) != 0);

  return NULL;
}

int main() {
  pthread_t tr, te, td, tw;

  tr2te = sem_open("/tr2teSem", O_CREAT, 0666, 0);

//  if (sem_init(&tr2te, 0,1)) {
//    printf("init");
//  }

//  sem_init(&tr2te, 0, 0);  // TODO[ml] catch errors
  init_queue(&q);

  pthread_create(&tr, NULL, &trFunction, NULL);
  pthread_create(&te, NULL, &teFunction, NULL);


  pthread_join(tr, NULL);
  pthread_join(te, NULL);

  sem_close(tr2te);
/*
  pthread_create(&td, NULL, &tdFunction, NULL);
  pthread_join(td, NULL);

  pthread_create(&tw, NULL, &twFunction, NULL);
  pthread_join(tw, NULL);
*/
  return 0;
}
