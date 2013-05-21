#ifndef LOG_H
#define LOG_H

void log_init();
void log_post(char * message, char * ident);
void log_close();

#endif
