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
#include <getopt.h>

char *r, *se, *sd;
sem_t * tr2te;
sem_t * te2td;
sem_t * td2tw;
sem_t * tw2te;
queue q;
queue_item globalQi;
int quit = 0, sleep_time = 0;

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
  char *log;
  char *message = "SD generated: ";
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
    log = (char *) malloc((strlen(message) + globalQi.size + 1) * sizeof(char));
    strcpy(log, message);
    strncat(log, sd, globalQi.size);
    log_post(log, "raxor_td");
    free(log);

    free(se);
    free(r);
    sem_post(td2tw);
  }
}

void *teFunction (void * ptr) {
  int i, rfd;
  char c;
  char *log;
  char *end = "quit";
  char *firstMessage = "R generated: ";
  char *secondMessage = "SE generated: ";

  log_post("Thread TE init", "raxor_te");
  
  while(1) {
    sem_wait(tw2te);
    sem_wait(tr2te);

    sleep(sleep_time);
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

    log = (char *) malloc((strlen(firstMessage) + globalQi.size + 1) * sizeof(char));
    strcpy(log, firstMessage);
    strncat(log, r, globalQi.size);
    log_post(log, "raxor_te");
    free(log); 

    close(rfd);

    log_post("Generate SE string", "raxor_te");
    se = getXOR(globalQi.s, r, globalQi.size);

    log = (char *) malloc(strlen(secondMessage) + globalQi.size + 1);
    strcpy(log, secondMessage);
    strncat(log, se, globalQi.size);
    log_post(log, "raxor_te");
    free(log); 
    
    log_post("Print R and SE", "raxor_te");
    printf("\nR: %.*s\n", globalQi.size, r);
    printf("SE: %.*s\n", globalQi.size, se);
    sem_post(te2td);
  }
}

void *trFunction (void * ptr) {
  int size;
  char * s = (char *) malloc (N_BYTES + 1);
  char * message = "Create a new queue item: ";
  char * log;
  char * end = "quit";
  queue_item qi;

  log_post("Thread TR init", "raxor_tr");

  while(1) {
    s = (char *) malloc (N_BYTES + 1);

    size = getline(&s, &N_BYTES, stdin) - 1;
    log_post("Reading a new string", "raxor_tr");
   

    log = (char *) malloc((strlen(message) + size + 1) * sizeof(char));
    strcpy(log, message);
    strncat(log, s, size); 
    log_post(log, "raxor_tr");
    free(log);

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
void print_usage() {
    printf("Usage: encodes and decodes strings -s seconds\n");
    printf("\tExit with 'quit'\n");
}

int main(int argc, char** argv) {
  int opt, long_index=0;
  pthread_t tr, te, td, tw;
/*
  while ( (c = getopt(argc, argv, "h")) != -1 ) {
    switch(c) {
      case 'h':
        printf("code: encodes and decodes strings\n");
        printf("\tUsage: code\n");
        printf("\tQuit with 'quit'\n");
        return 0;
    }
  }*/

   static struct option long_options[] = {
        {"help",      no_argument,       0,  'h' },
        {"second",    required_argument, 0,  's' },
        {0,           0,                 0,  0   }
    };

    while ((opt = getopt_long_only(argc, argv,"", long_options, &long_index )) != -1) {
        switch (opt) {
           case 'h' :
             print_usage();
             return 0;
           case 's' :
             if (optarg != NULL)
               sleep_time = atoi(optarg);
             break;
           default: print_usage();
             break;
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
