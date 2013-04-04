#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>

size_t nbytes = 1000;

void *teFunction (void * ptr) {
  // pthread_create(&tr, NULL, &trFunction, NULL);
}

void *trFunction (void * ptr) {
  printf("$> ");
  return NULL;
}

char * getXOR(char * s1, char *s2, int size) {
  char * tmp = malloc(size * sizeof(char));
  int i;

  for (i = 0; i < size; i++)
    *(tmp+i) = s1[i] ^ s2[i];

  *(tmp+i) = '\0';
  return tmp;
}

int main() {
  int i, rfd;
  char c, *r, *se, *sd;
  pthread_t tr, te, td, tw;

  pthread_create(&tr, NULL, &trFunction, NULL);
  pthread_join(tr, NULL);
/*
  r = malloc(size * sizeof(char));

  rfd = open("/dev/random", O_RDONLY);

  for (i = 0; i < size - 1; i++) {
    read(rfd, &c, sizeof(char));
    *(r+i) = abs(c) % 26 + 65;
  }
  *(r+i) = '\0';
  close(rfd);

  se = getXOR(s, r, size);
  sd = getXOR(r, se, size);

  printf("INPUT: %s\n", s);
  printf("R: %s\n", r);
  printf("SE: %s\n", se);
  printf("SD: %s\n", sd);
*/
  return 0;
}
