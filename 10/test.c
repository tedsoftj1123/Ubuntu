#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signum) {
	printf("bye\n");
}
int main() {
	pid_t pid;
	
	struct sigaction act;
	sigfillset(&(act.sa_mask));
	act.sa_handler = handler;
	sigaction(SIGINT, &act, NULL);
	if((pid = fork()) > 0) {
		pause();
		printf("good bye child\n");
	} else if(pid == 0) {
		sleep(1);
		printf("ready");
		kill(getppid(), SIGINT);
		printf("bye mom\n");
	}

	return 0;
}
