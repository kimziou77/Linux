#include "MyStudent.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#define PERMS 0644

int main(int argc, char const *argv[]) {
	int fd=0;
	char * pathName="./student.dat.";//save path
	
	size_t wSize=0;

	char userID[MAX_ID_SIZE+1]={'\0',};//id
	char userName[MAX_NAME_SIZE+1]={'\0',};//name
	int userScore=0;//socre

	//create Student struct to save userinfo
	Student * st = (Student *)malloc(sizeof(Student));//allocate
	
	memset(st->id,'\0',MAX_NAME_SIZE+1); //insert nullchar
	memset(st->name,'\0',MAX_NAME_SIZE+1);
	
	//input data
	printf("ID : ");	scanf("%s",userID);
	strcpy(st->id,userID);
	printf("NAME : ");	scanf("%s",userName);
	strcpy(st->name,userName);
	printf("Score : ");	scanf("%d",&userScore);
	st->score=userScore;
	
	//open file to save userInfo
	fd=open(pathName,O_CREAT|O_APPEND|O_RDWR,PERMS);
	if(fd==-1){
		perror("open() error!");
		exit(-1);
	}
	
	wSize = write(fd,(Student *)st,sizeof(Student));
	if(wSize==-1){
		perror("write() error!");
		exit(-2);
	}
	
	//printf("fd : %d, wsize: %ld \n",fd,wSize);
	printf("Saved at ./student.dat.\n");
	close(fd);
	free(st);

    return 0;
}
