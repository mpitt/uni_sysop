#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include "log.h"
#include "queue.h"
#include "code.h"

char *r, *se, *sd;
sem_t * tr2te;
sem_t * te2td;
sem_t * td2tw;
sem_t * tw2te;
queue q;
queue_item globalQi;
int quit = 0;

char *getXOR(char *s1, char *s2, int size) {
  int i;
  char * tmp = malloc(size * sizeof(char));

  for (i = 0; i < size; i++)
    *(tmp+i) = s1[i] ^ s2[i];

  return tmp;
}

void *twFunction(void * ptr) {
  log_post("Thread TW init", "raxor_tw");
  while(1) {
    sem_wait(td2tw);
    if (quit) {
      log_post("Thread TW close", "raxor_tw");
      sem_post(tw2te);
      return NULL;
    }

    log_post("Print SD", "raxor_tw");
    printf("SD: %.*s\n", globalQi.size, sd);
    free(sd);
    sem_post(tw2te);
  }
}

void *tdFunction (void * ptr) {
  log_post("Thread TD init", "raxor_td");
  while(1) {
    sem_wait(te2td);

    log_post("Generate SD string", "raxor_td");
    if (quit) {
      log_post("Thread TD close", "raxor_td");
      sem_post(td2tw);
      return NULL;
    }
    sd = getXOR(r, se, globalQi.size);
    char log[] = "SD generated: ";
    strcat(log, sd);
    log_post(log, "raxor_te");

    free(se);
    free(r);
    sem_post(td2tw);
  }
}

void *teFunction (void * ptr) {
  int i, rfd;
  char c;
  char * end = "quit";

  log_post("Thread TE init", "raxor_te");
  
  while(1) {
    sem_wait(tw2te);
    sem_wait(tr2te);

    sleep(2);
    globalQi = dequeue(&q);
    log_post("Dequeue queue item", "raxor_te");
    
    if (strncmp(globalQi.s, end, strlen(end)) == 0) {
      log_post("Thread TE close", "raxor_te");
      quit = 1;
      sem_post(te2td);
      return NULL;
    }

    r = malloc((globalQi.size) * sizeof(char));
    rfd = open("/dev/random", O_RDONLY);

    log_post("Generate random R string", "raxor_te");
    for (i = 0; i < globalQi.size; i++) {
      read(rfd, &c, sizeof(char));
      *(r+i) = abs(c) % 26 + 65;
    }

    char logR[] = "R generated: ";
    strcat(logR, r);
    log_post(logR, "raxor_te");

    close(rfd);

    log_post("Generate SE string", "raxor_te");
    se = getXOR(globalQi.s, r, globalQi.size);
    
    char logSe[] = "SE generated: ";
    strcat(logSe, se);
    log_post(logSe, "raxor_te");
    
    log_post("Print R and SE", "raxor_te");
    printf("\nR: %.*s\n", globalQi.size, r);
    printf("SE: %.*s\n", (int) strlen(se), se);
    sem_post(te2td);
  }
}

void *trFunction (void * ptr) {
  int size;
  char * s = (char *) malloc (N_BYTES + 1);
  char * end = "quit";
  queue_item qi;

  log_post("Thread TR init", "raxor_tr");

  while(1) {
    s = (char *) malloc (N_BYTES + 1);

    size = getline(&s, &N_BYTES, stdin) - 1;
    log_post("Reading a new string", "raxor_tr");
   

    char log[] = "Create a new queue item: ";
    strcat(log, s); 

    log_post(log, "raxor_tr");
    qi.s = s;
    qi.size = size;
  
    log_post("Enqueue queue item", "raxor_tr");
    enqueue(&q, &qi);

    sem_post(tr2te);

    if (strncmp(s, end, strlen(end)) == 0) {
      log_post("Thread TR close", "raxor_tr");
      return NULL;
    }
  }
}

int main(int argc, char** argv) {
  int c;
  pthread_t tr, te, td, tw;

  while ( (c = getopt(argc, argv, "h")) != -1 ) {
    switch(c) {
      case 'h':
        printf("code: encodes and decodes strings\n");
        printf("\tUsage: code\n");
        printf("\tQuit with 'quit'\n");
        return 0;
    }
  }

  log_init();

  /* remove useless semaphore */
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

  log_close();

  return 0;
}
