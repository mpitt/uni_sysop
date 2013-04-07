#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>

size_t nbytes = 1000;
int size;
int i, rfd;
char c, *s, *r, *se, *sd;

char * getXOR(char * s1, char *s2, int size) {
  char * tmp = malloc(size * sizeof(char));
  int i;

  for (i = 0; i < size; i++)
    *(tmp+i) = s1[i] ^ s2[i];

  *(tmp+i) = '\0';
  return tmp;
}

void *twFunction(void * ptr) {
  printf("SD: %s\n", sd);
}

void *tdFunction (void * ptr) {
   sd = getXOR(r, se, size);
}

void *teFunction (void * ptr) {
  r = malloc(size * sizeof(char));

  rfd = open("/dev/random", O_RDONLY);

  for (i = 0; i < size - 1; i++) {
    read(rfd, &c, sizeof(char));
    *(r+i) = abs(c) % 26 + 65;
  }
  *(r+i) = '\0';
  close(rfd);
  printf("R: %s\n", r);
  se = getXOR(s, r, size);
  printf("SE: %s\n", se);
}

void *trFunction (void * ptr) {
  s = (char *) malloc (nbytes + 1);

  printf("$> ");
  size = getline(&s, &nbytes, stdin);

  return NULL;
}

int main() {
  pthread_t tr, te, td, tw;

  pthread_create(&tr, NULL, &trFunction, NULL);
  pthread_join(tr, NULL);

  pthread_create(&te, NULL, &teFunction, NULL);
  pthread_join(te, NULL);

  pthread_create(&td, NULL, &tdFunction, NULL);
  pthread_join(td, NULL);

  pthread_create(&tw, NULL, &twFunction, NULL);
  pthread_join(tw, NULL);
  return 0;
}
