#include<signal.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int num = 0;

void int_handle(int signum) {
	printf("SIGINT:%d\n", signum);
	printf("int_handler called %d times\n", ++num);
}
int main() {
	static struct sigaction act;

	act.sa_handler = int_handle;
	sigfillset(&(act.sa_mask));
	sigaction(SIGINT, &act, NULL);

	while(1) {
		printf("asdf\n");
		sleep(1);
		if(num >= 2) {
			act.sa_handler = SIG_DFL;
			sigaction(SIGINT, &act, NULL);
		}
	}
}
