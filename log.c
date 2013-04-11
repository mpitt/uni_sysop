#include "log.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

char * LOG_APPEND = "a";
char * LOG_DIR = "log";

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
  fputs(message, log);
  LogClose(log);
}
