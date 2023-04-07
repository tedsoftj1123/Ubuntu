#include<stdio.h>
#include<unistd.h>

int main() {
	char *arg[] = {"ls", "-l", (char *)0};
	execv("/bin/ls", arg);
	printf("after");
	return 0;
}
