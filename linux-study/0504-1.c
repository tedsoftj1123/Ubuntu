#include<sys/types.h>
#include<unistd.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>

int main() {
	DIR *dirp;
	struct dirent *dentry;

	if((dirp=opendir("."))==NULL)
		exit(1);

	while(dentry = readdir(dirp)){
		if(dentry->d_ino != 0)
			printf("%s\n", dentry->d_name);
	}

	rewinddir(dirp);
	printf("delete files..\n");
	while(dentry = readdir(dirp)) {
		if(dentry->d_ino == 0)
			printf("%s\n", dentry->d_name);
	}

	closedir(dirp);
}
