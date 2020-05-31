#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "disk.h"

int fd; 

void DevCreateDisk(void)
{
    fd = open("MY_DISK", O_RDWR | O_CREAT | O_TRUNC, 0644);
    /*
    O_RDWR : 파일을 읽고 쓰기를 모두 할 수 있도록 open
    O_CREAT : 이 옵션은 파일이 존재하지 않으면 파일을 새로 만들어라는 설정입니다. 이 옵션이 설정되면 mode를 입력받는 open(const char *pathname, int flags, mode_t mode);를 호출해야 합니다. 즉, 파라미터가 3개인 open(2)함수를 호출해야 합니다.
    O_TRUNC : O_WRONLY 또는 O_RDWR로 open시에 이미 존재하는 파일이면 
    파일의 내용을 삭제하여 파일의 크기를 0으로 만듭니다.
    Open System Call Flag : https://www.it-note.kr/19
    */
}

void DevOpenDisk(void)
{
	fd = open("MY_DISK", O_RDWR);
}

void __DevMoveBlock(int blkno){
    lseek(fd, (off_t)+(BLOCK_SIZE*blkno),SEEK_SET);
}

void DevReadBlock(int blkno, char* pBuf)
{
   __DevMoveBlock(blkno);
   read(fd, pBuf, BLOCK_SIZE);
}

void DevWriteBlock(int blkno, char* pBuf)
{
   __DevMoveBlock(blkno);
   write(fd, pBuf, BLOCK_SIZE);
}


void DevCloseDisk(void)
{
    close(fd);
}
