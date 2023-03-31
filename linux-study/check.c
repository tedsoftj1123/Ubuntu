#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#define BUF_SIZE 256

void printcwd(char dirPos)
{
	char buffer[BUF_SIZE];

	if(getcwd(dirPos, BUF_SIZE) == NULL)
		printf("not exists");
		exit(1);

	printf("%s\n", buffer);
}

int main(int argc, char** argv) {
	printcwd(argv[0]);
}
