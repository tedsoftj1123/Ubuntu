#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
int main() {
	int filedes;
	char pathname[] = "temp.txt";

	if((filedes = open(pathname, O_CREAT | O_RDWR, 0644)) == -1) {
		printf("error\n");
		exit(1);
	}

	close(filedes);
}
