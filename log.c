#include "log.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

char * LOG_APPEND = "a";
char * LOG_DIR = "log";

char * GetCurrentTime() {
  time_t ltime;
  struct tm result = {0};
  char * s_time = (char*)malloc(20 * sizeof(char));

  ltime = time(NULL);
  localtime_r(&ltime, &result);
  strftime(s_time, 50, "%b %d %H:%M:%S :", &result);
  return s_time;

}

void LogCheckDir() {
  struct stat st = {0};
  if (stat(LOG_DIR, &st) == -1) {
    mkdir(LOG_DIR, 0755);
  }
}

FILE * LogOpen(char * file) {
  LogCheckDir();
  char path[255];
  strcpy(path, LOG_DIR);
  strcat(path, "/");
  strcat(path, file);
  FILE * fd = fopen(path, LOG_APPEND);
  return fd;
}

void LogClose(FILE * fd) {
  fclose(fd);
}

void Log(char * message, char * file) {
  FILE * log = LogOpen(file);
  char * timestamp = GetCurrentTime();
  strcat(timestamp, " ");
  strcat(timestamp, message);
  fputs(timestamp, log);
  LogClose(log);

//Apr 10 08:22:33 M14x syslog-ng[275]:
}
