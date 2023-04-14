#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>

int main() {
	pid_t pid;
	int status = 0;

	if ((pid = fork()) > 0) {
		while(!waitpid(pid, &status, WNOHANG)) {
			printf("parent: %d\n", status++);
			sleep(1);
		}
		printf("asdf %d\n", status);
	} else if (pid == 0) {
		sleep(5);
		printf("bye\n");
		exit(0);
	} else {
		printf("filaed");
	}

	return 0;
}
