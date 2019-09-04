#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PERMS 0644

int main (void) {
	int fd=0;
	char * pathname= "./newfile.txt";
	
	//fd = creat(pathname, PERMS);
	fd= open(pathname,O_CREAT | O_RDWR,PERMS);
	
	if(fd==-1){
		perror("open() error!");
		exit(-1);
	}

	printf("fd of %s is : %d\n",pathname,fd);
	close(fd);
	printf("hello \n");

	return 0;
}
	
