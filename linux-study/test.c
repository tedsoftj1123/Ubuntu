#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int sum;
int main() {
	int n;
	scanf("%d", &n);

	pid_t pid;

	pid = fork();

	if (pid > 0)
		printf("%d", sum);
	} else if(pid == 0) {
		printf("%d", sum);
	}
}
