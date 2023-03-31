#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

int main() {
	int fd = open("./test.txt", O_RDWR, 0644);
	char input[10];
	fgets(input, 10, stdin);

	write(fd, input, 10);
	close(fd);

	return 0;
}
