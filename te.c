#include "te.h"

void *teFunction (void * ptr) {

/* teFunction
 *
 * Wait until semaphore 'tw2te' and 'tr2te' is unlocked,
 * then dequeue an item, read a string from /dev/random (r),
 * write a log message, create SE, print r and SE and unlock 'te2td'.
 * If dequeued item is 'quit' set quit to 1, unlock 'te2td',
 * write a log message and return NULL.
 */

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
