#include "tw.h"

void *twFunction(void * ptr) {
/* twFunction wait until semaphore 'td2tw' is unlocked, then print SD and unlock tw2te semaphore.
   If 'quit' is 1, write a log message and return NULL
*/
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
