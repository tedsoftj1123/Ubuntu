#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main() {
	int n;
	scanf("%d", &n);

	pid_t pids[n];

	while(n--) {
		pids[n] = fork();
		if(pids[n] == 0) {
			printf("child(PID: %d, PPID: %d)\n", getpid(), getppid());
		} else if(pids[n] < 0) {
			printf("error\n");
		} else
			printf("parent(PID: %d)\n", getpid());
	}
}
