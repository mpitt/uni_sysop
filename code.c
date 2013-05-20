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
sem_t * te2td;
sem_t * td2tw;
sem_t * tw2te;
queue q;
int quit = 0;


char * getXOR(char * s1, char *s2, int size) {
  int i;
  char * tmp = malloc(size * sizeof(char));

  for (i = 0; i < size -1; i++)
    *(tmp+i) = s1[i] ^ s2[i];

  return tmp;
}

void *twFunction(void * ptr) {
  while(1) {
    sem_wait(td2tw);
    if (quit) {
      sem_post(tw2te);
      return NULL;
    }
    printf("SD: %s\n", sd);
    free(sd);
    sem_post(tw2te);
  }
  return NULL;
}

void *tdFunction (void * ptr) {
  while(1) {
    sem_wait(te2td);
    if (quit) {
      sem_post(td2tw);
      return NULL;
    }
    sd = getXOR(r, se, size);
    free(se);
    sem_post(td2tw);
  }
  return NULL;
}

void *teFunction (void * ptr) {
  int i, rfd;
  char c;
  queue_item qi;

  while(!quit) {
    sem_wait(tw2te);

    if( (is_empty(&q)) && (!quit)) {
      sem_wait(tr2te);
      if (quit) {
        sem_post(te2td);
        return NULL;
      }
    }
    sleep(2);
    qi = dequeue(&q);

    r = malloc(size * sizeof(char));

    rfd = open("/dev/random", O_RDONLY);

    for (i = 0; i < size - 1; i++) {
      read(rfd, &c, sizeof(char));
      *(r+i) = abs(c) % 26 + 65;
    }

    close(rfd);

    se = getXOR(qi.s, r, size);

    printf("R: %s\n", r);
    printf("SE: %s\n", se);
    sem_post(te2td);
  }
  return NULL;
}

void *trFunction (void * ptr) {
  queue_item qi;
  char * s = (char *) malloc (N_BYTES + 1);
  char * end = "quit";

  while(!quit) {
//  printf("$> ");
    free(s);
    s = (char *) malloc (N_BYTES + 1);
    
    size = getline(&s, &N_BYTES, stdin);
    printf("%u", &s);
    s[strlen(s)-1] = '\0';  // remove last char of this string
    if (strcmp(s, end) != 0) { 
      strcpy(qi.s, s);
      qi.size = size;
      enqueue(&q, &qi);
    } else { 
      quit = 1;
    }    
    sem_post(tr2te);
  }
  return NULL;
}

int main() {
  pthread_t tr, te, td, tw;

  // remove useless semaphore
  sem_unlink("/tr2teSem");
  sem_unlink("/te2tdSem");
  sem_unlink("/td2twSem");
  sem_unlink("/tw2teSem");

  tr2te = sem_open("/tr2teSem", O_CREAT|O_EXCL, 0666, 0);
  te2td = sem_open("/te2tdSem", O_CREAT|O_EXCL, 0666, 0);
  td2tw = sem_open("/td2twSem", O_CREAT|O_EXCL, 0666, 0);
  tw2te = sem_open("/tw2teSem", O_CREAT|O_EXCL, 0666, 0);

  sem_post(tw2te);
  init_queue(&q);

  pthread_create(&tr, NULL, &trFunction, NULL);
  pthread_create(&te, NULL, &teFunction, NULL);
  pthread_create(&td, NULL, &tdFunction, NULL);
  pthread_create(&tw, NULL, &twFunction, NULL);

  pthread_join(tr, NULL);
  pthread_join(te, NULL);
  pthread_join(td, NULL);
  pthread_join(tw, NULL);

  sem_close(tr2te);
  sem_close(te2td);
  sem_close(td2tw);
  sem_close(tw2te);

  return 0;
}
