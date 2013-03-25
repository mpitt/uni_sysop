#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

char * getXOR(char * s1, char *s2, int size) {
  char * tmp = malloc(size * sizeof(char));
  int i;

  for (i = 0; i < size; i++)
    *(tmp+i) = s1[i] ^ s2[i];

  *(tmp+i) = '\0';
  return tmp;
}

int main() {
  char s[] = "ciao lumachina puppa, ma anche bellissima";
  int size = sizeof s;
  char * r = malloc(size * sizeof(char));
  char * se;
  char * sd;
  int i;
  char c;

  int rfd = open("/dev/random", O_RDONLY);

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

  return 0;
}
