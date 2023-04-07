#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

void final(void) {
	printf("close filed\n");
	close(filedes);
}

int main() {
	ssize_t nread;
	char buffer[1024];

	atexit(final);

	fildes = open("a.txt", O_RDONLY);
	while((nread = read(filedes, buffer, 1024)) > 0) {
		printf("%s", buffer);
	}
}
