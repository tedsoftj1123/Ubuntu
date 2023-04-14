#include<unistd.h>

int main() {
	char *envlist[] = {"APPLE=0", "BANANA=1", (char *)0};

	execle("ex0813", "ex0813", (char *)0, envlist);
}
