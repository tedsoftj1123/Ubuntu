#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>

int main() {
	int fd;
	char pathName[] = "test.txt";

	if((fd==open(pathName, O_CREAT | O_RDWR, 0644)) == -1) {
		printf("Error\n");
		exit(1);
	}
	
	close(fd);
}
