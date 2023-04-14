#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<math.h>

int main() {
	pid_t pid;
	int status = 0;

	int a, b;
	scanf("%d %d", &a, &b);

	int pAns = 0, cAns = 0;

	if((pid = fork()) > 0) {
		for (int i=0;i<b;i++) {
			a *= a;
		}
		pAns = a;

		wait(&cAns);
		cAns = cAns >> 8;
		printf("pAns + cAns = %d", pAns + cAns);
	} else if(pid==0) {
		for (int i = a; i < b; i++) {
			cAns += i;
		}
		exit(cAns);
	}

	return 0;
}
