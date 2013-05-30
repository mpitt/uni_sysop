#include "tr.h"

void *trFunction (void * ptr) {
/* trFunction read a string from stdin, put it into a queue and unlock a 'tr2te' semaphore.
   If the string from stdin is 'quit', write a log message and return NULL
*/
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
