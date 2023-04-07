#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>

int i = 2;
int main() {
	pid_t pid;
	i++;

	printf("Before Fork: %d\n", i);

	pid = fork();

	if(pid == 0)
		printf("child process(%d)\n", ++i);
	else if(pid > 0)
		printf("parent process(%d)\n", --i);
	else
		printf("failed");

	return 0;
}
