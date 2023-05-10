#include <stdio.h>
extern char **environ;
int main(char *argv[]) {
    while (*environ)
        printf("%s\n", *environ++);
}
