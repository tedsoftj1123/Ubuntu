#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int status;
    char *wc_argv[3] = {"wc", NULL, NULL};

    for (int i = 1; i < argc; i++) {
        pid = fork();
        if (pid == 0) {
            wc_argv[1] = argv[i];
            execvp("wc", wc_argv);
            exit(1);
        }
        else if (pid > 0) {
            waitpid(pid, &status, 0);
        }
        else {
            printf("failed\n");
            exit(1);
        }
    }

    return 0;
}
