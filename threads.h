#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>

#include "queue.h"

char *r, *se, *sd;
queue q;
queue_item globalQi;

pthread_t tr, te, td, tw;

sem_t * tr2te;
sem_t * te2td;
sem_t * td2tw;
sem_t * tw2te;

int quit, sleep_time;

char *getXOR(char *s1, char *s2, int size);

size_t N_BYTES = 1000;

void semInit();
void semClose();
void threadInit();
void threadClose();
