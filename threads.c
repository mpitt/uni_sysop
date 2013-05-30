#include <pthread.h>

#include "threads.h"
#include "tr.h"
#include "te.h"
#include "td.h"
#include "tw.h"


char *getXOR(char *s1, char *s2, int size) {

  /* Return a string 'tmp' created by the XOR between 's1' and 's2' */

  int i;
  char * tmp = malloc(size * sizeof(char));

  for (i = 0; i < size; i++)
    *(tmp+i) = s1[i] ^ s2[i];

  return tmp;
}

void semInit() {

  /* Initialize all the semaphores:
   * - unlink (remove) all the semaphores
   * - create (open) all the semaphore
   */

  N_BYTES = 1000;

  sem_unlink("/tr2teSem");
  sem_unlink("/te2tdSem");
  sem_unlink("/td2twSem");
  sem_unlink("/tw2teSem");

  tr2te = sem_open("/tr2teSem", O_CREAT|O_EXCL, 0666, 0);
  te2td = sem_open("/te2tdSem", O_CREAT|O_EXCL, 0666, 0);
  td2tw = sem_open("/td2twSem", O_CREAT|O_EXCL, 0666, 0);
  tw2te = sem_open("/tw2teSem", O_CREAT|O_EXCL, 0666, 0);

  sem_post(tw2te);
}

void semClose() {

  /* Close all the semaphores */

  sem_close(tr2te);
  sem_close(te2td);
  sem_close(td2tw);
  sem_close(tw2te);
}

void threadInit() {

  /* Create the four threads.
   * Thread's parameters are: name, options, function, arg
   */

  pthread_create(&tr, NULL, &trFunction, NULL);
  pthread_create(&te, NULL, &teFunction, NULL);
  pthread_create(&td, NULL, &tdFunction, NULL);
  pthread_create(&tw, NULL, &twFunction, NULL);
}

void threadClose() {

  /* Close (join) all the four threads */

  pthread_join(tr, NULL);
  pthread_join(te, NULL);
  pthread_join(td, NULL);
  pthread_join(tw, NULL);
}

