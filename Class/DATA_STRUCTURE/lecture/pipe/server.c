#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PERMS 0666
#define MAX_BUF_SIZE 32+1
int main(int argc, char const *argv[])
{
    int fd=0;
    char buf[MAX_BUF_SIZE]={0x00,};
    if(argc!=2){
        printf("Usage: %s [pathName]\n",argv[0]);
        return -1;
    }

    mkfifo(argv[1],PERMS);
    fd= open(argv[1],O_RDONLY);
    while(1){
        if(read(fd,(char*)buf,MAX_BUF_SIZE)>0){
            puts(buf);
            memset(buf,0x00,MAX_BUF_SIZE);
        }
    }
    return 0;
}
