#include "td.h"

void *tdFunction (void * ptr) {
/* tdFunction 
 * 
 * Wait until semaphore 'te2td' is unlocked, then create SD,
 * write a log message and unlock td2tw semaphore.
 * If 'quit' is 1, write a log message, unlock td2tw and return NULL.
 */

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

