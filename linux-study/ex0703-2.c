#include<unistd.h>
#include<stdio.h>

int main() {
	printf("before \n");
	execl("/bin/ls", "ls", "-l", (char *)0);
	printf("after executing ls -l\n");
}
