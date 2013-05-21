#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <syslog.h>
#include "log.h"

sem_t * thlog;

void log_init() {
  sem_unlink("/thlogSem");
  thlog = sem_open("/thlogSem", O_CREAT|O_EXCL, 0666, 0);
  sem_post(thlog);
}

void log_post(char * message, char * ident) {
  sem_wait(thlog);
  openlog(ident, LOG_PID, LOG_USER);
  syslog(LOG_INFO, "%s", message);
  closelog();
  sem_post(thlog);
}

void log_close() {
  sem_close(thlog);
}
