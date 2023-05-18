#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handler(int signum) {
	printf("\nSIGINT cought\n\n");
}

int main() {
	struct sigaction act;

	sigfillset(&(act.sa_mask));
	act.sa_handler = handler;
	sigaction(SIGINT, &act, NULL);

	printf("pause return %d\n", pause());
}
