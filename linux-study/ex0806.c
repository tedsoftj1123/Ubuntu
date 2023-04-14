#include<unistd.h>
#include<sys/types.h>

int main() {
	putenv("APPLE=BANANA");
	printf("%s\n", getenv("APPLE"));

	execl("ex0811", "ex0811", (char *)0);
}
