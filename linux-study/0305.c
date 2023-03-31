#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

int main(int argc, char **argv) {
	int filedes;
	off_t newpos;

	filedes = open(argv[1], O_RDONLY);

	newpos = lseek(filedes, (off_t_0, SEEK_END);

	printf("file size: %d\n", newpos);	
	return 0;
}
