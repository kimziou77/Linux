#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include "disk.h"
#include "fs.h"
#define MODE 0644
#define BLOCK_NUM 512
#define DEBUGGING 1
enum {FAILED=-1, SUCCESS};
enum {FALSE=0, TRUE=1};
enum {PARENT=0, CHILD};
int WhereIsInodeBlock(int indoeno);
int WhereIsInodeOrder(int indoeno);
int WhereNewDirOrder(DirEntry * direntry,char * szDirName);
int FindDirTable(DirEntry * direntry, char * targetName);
int FindFileDescripterTable();
int pathFinder(const char * szFileName);
int pathFinder_n(const char * szFileName);
int FindOffsetBlock(int legnth);
/*
/static으로 숫자를 세어주고 num-of-dirent-per-block 보다 커지면.. 다시0으로
    //절대경로니까 항상 /로 시작 -> szName
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(0,pInode);//루트 Dir을 제일 먼저 읽어온다.


    DirEntry* dirEntry = (DirEntry *)pBuf;

    int up;
    //디렉토리 엔트리 살펴보고 디렉토리엔트리 살펴보고..
    char parsing[100];//input이 100이 안넘는다고 가정...
	strcpy(parsing, szDirName);
	char* ptr = strtok(parsing, "/");
	while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
	{
        //ptr에 대한 DirTable인덱스
        int en = FindDirTable(dirEntry,ptr);
        if(en==FAILED) break;   //못 찾았으면 일단 break... 마지막엔 원래 Fail인데..
        
        int enI = dirEntry[en].inodeNum;//ptr에 대한 inode
        GetInode(enI,pInode);//inode를 가지고와서 dirEntry
        up = pInode -> dirBlockPtr[0];//ptr의 블록번호를 가지고 온다.
        
        DevReadBlock(up,pBuf);//ptr블럭을 가지고 온다.
        dirEntry = (DirEntry *)pBuf;
        
		ptr = strtok(NULL, "/");      // 다음 문자열을 잘라서 포인터를 반환
	}
    //direnty에 상위directory에 대한 block이 있을것임.
    //TODO: break됐을때 마지막 token이 아닌경우 예외처리 해주기
*/
