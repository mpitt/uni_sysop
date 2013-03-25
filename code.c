#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    char s[] = "ciao ciao";
    char * r = malloc(sizeof(s) * sizeof(char));
    int i;
    char c;
    int rfd = open("/dev/random", O_RDONLY);
    for (i = 0; i < sizeof(s) - 1; i++) {
        read(rfd, &c, sizeof(char));
        printf("%d ", abs(c)); 
        *(r+i) = abs(c) % 26 + 65;
    }
    *(r+i) = '\0';
    close(rfd);
    printf("%s\n", s);
    printf("%s\n", r);
    return 0;
}
