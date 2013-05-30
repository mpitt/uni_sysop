/* Università degli Studi di Trento
 * A.A. 2012/2013 - Corso di Laurea in Informatica
 * Sistemi operativi 1
 * Progetto 2: Threads
 *
 * Autori:
 * Mattia Larentis 151889
 * Andrea Panizza 151887
 * Michele Pittoni 152549
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <semaphore.h>
#include <syslog.h>
#include "log.h"

sem_t * thlog;

void log_init() {
/* Unlink and create the log's semaphore */
  sem_unlink("/thlogSem");
  thlog = sem_open("/thlogSem", O_CREAT|O_EXCL, 0666, 0);
  sem_post(thlog);
}

void log_post(char * message, char * ident) {
/* Wait until the semaphore 'thlog' is unlocked,
   then write a message with the default log's library and 
   unlock 'thlog' semaphore
*/
  sem_wait(thlog);
  openlog(ident, LOG_PID, LOG_USER);
  syslog(LOG_INFO, "%s", message);
  closelog();
  sem_post(thlog);
}

void log_close() {
/* Close the log semaphore */
  sem_close(thlog);
}
