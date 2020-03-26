#include "MyStudent.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
int main(int argc, char const *argv[]) {
	//var init
	int fd=0;
	char * pathName="./student.dat.";
	size_t rSize=0;
	
	//save data container
	Student * st= (Student *)malloc(sizeof(Student));
	memset(st->id,'\0',MAX_ID_SIZE+1);
	memset(st->name,'\0',MAX_NAME_SIZE+1);
	st->score=0;

	//file open
	fd=open(pathName,O_RDONLY);
	if(fd==-1){
		perror("open() error!");
		exit(-1);
	}
	
	//read file

	do{
		rSize=read(fd,(Student *)st, sizeof(Student));
		if(rSize==-1){
			perror("read() error!");
			exit(-2);
		}
		if(rSize==0)	break;
		//printf("fd : %d, rSize: %ld \n",fd,rSize);
		printf("Student ID : %s, Name : %s , Score : %d\n",st->id, st->name,st->score);

	}while(1);

	//final
	close(fd);
	free(st);
    return 0;
}
