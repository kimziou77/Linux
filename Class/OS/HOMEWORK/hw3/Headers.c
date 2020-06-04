#include "Headers.h"

int pathFinder(const char * szFileName){
    /*szFileName의 마지막경로의 부모에 대한 Inode를 반환*/
    // Root Directory 불러오기
    // 디렉토리를 탐색해 나간다 ex) /dir1/dir2/dir3
    // "/" 기준으로 토크나이징한다.
    // 현재 token의 inode에서 다음토큰 이름을 찾아 해당 inode를 찾는다를 반복
    /***************************************************/
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(0,pInode);//절대경로 -> 루트부터 시작

    DirEntry* dirEntry = (DirEntry *)pBuf;
    int ptrTableIndex;      //ptr에 대한 DirTable인덱스
    int ptrInodeNum;        //ptr에 대한 inode
    int ptrBlock;           //ptr블럭

    char parsing[100];//input이 100이 안넘는다고 가정...
	strcpy(parsing, szFileName);
	char* ptr = strtok(parsing, "/");
	while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
	{
        ptrTableIndex = FindDirTable(dirEntry,ptr);//디렉토리에서 ptr찾기
        if(ptrTableIndex==FAILED) break;   //못 찾았으면 break...
        ptrInodeNum = dirEntry[ptrTableIndex].inodeNum;
        GetInode(ptrInodeNum,pInode);                   //inode를 가지고 온다.
        ptrBlock = pInode -> dirBlockPtr[0];            //ptr의 자신의 블록번호를 가지고 온다.
        DevReadBlock(ptrBlock,pBuf);                    //ptr블럭을 가지고 온다.
        dirEntry = (DirEntry *)pBuf;
		ptr = strtok(NULL, "/");                        // 다음 ptr
	}
    free(pBuf);

    ptr = strtok(NULL,"/"); 
    if(ptr != NULL) return FAILED;//그 다음 token이 있었다면 Failed

    return ptrBlock;//생성파일의 상위디렉토리 블럭번호 반환
}

int pathFinder_n(const char * szFileName){
    /*szFileName의 마지막경로에 대한 Inode를 반환*/
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(0,pInode);//절대경로 -> 루트부터 시작

    DirEntry* dirEntry = (DirEntry *)pBuf;
    int ptrTableIndex;      //ptr에 대한 DirTable인덱스
    int ptrInodeNum;        //ptr에 대한 inode
    int ptrBlock;           //ptr블럭

    char parsing[100];//input이 100이 안넘는다고 가정...
	strcpy(parsing, szFileName);
	char* ptr = strtok(parsing, "/");
	while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
	{
        ptrTableIndex = FindDirTable(dirEntry,ptr);//디렉토리에서 ptr찾기
        if(ptrTableIndex==FAILED) return FAILED;   //못 찾았으면 failed...
        ptrInodeNum = dirEntry[ptrTableIndex].inodeNum;
        GetInode(ptrInodeNum,pInode);                   //inode를 가지고 온다.
        ptrBlock = pInode -> dirBlockPtr[0];            //ptr의 자신의 블록번호를 가지고 온다.
        DevReadBlock(ptrBlock,pBuf);                    //ptr블럭을 가지고 온다.
        dirEntry = (DirEntry *)pBuf;
		ptr = strtok(NULL, "/");                        // 다음 ptr
	}
    free(pBuf);
    return ptrInodeNum;
}
char* NameFinder(const char * szFileName){
    /*szFileName의 마지막경로에 대한 Inode를 반환*/
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(0,pInode);//절대경로 -> 루트부터 시작

    DirEntry* dirEntry = (DirEntry *)pBuf;
    int ptrTableIndex;      //ptr에 대한 DirTable인덱스
    int ptrInodeNum;        //ptr에 대한 inode
    int ptrBlock;           //ptr블럭

    char parsing[100];//input이 100이 안넘는다고 가정...
	strcpy(parsing, szFileName);
    char * name;
	char* ptr = strtok(parsing, "/");//TODO: 메모리가...어떻게 되려나..
	while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
	{
        ptrTableIndex = FindDirTable(dirEntry,ptr);//디렉토리에서 ptr찾기
        if(ptrTableIndex==FAILED) return FAILED;   //못 찾았으면 failed...
        ptrInodeNum = dirEntry[ptrTableIndex].inodeNum;
        GetInode(ptrInodeNum,pInode);                   //inode를 가지고 온다.
        ptrBlock = pInode -> dirBlockPtr[0];            //ptr의 자신의 블록번호를 가지고 온다.
        DevReadBlock(ptrBlock,pBuf);                    //ptr블럭을 가지고 온다.
        dirEntry = (DirEntry *)pBuf;
        name = ptr;
		ptr = strtok(NULL, "/");                        // 다음 ptr
	}
    free(pBuf);
    return name;
}

int FindFileDescripterTable(){
    for(int i=0; i<MAX_FD_ENTRY_MAX; i++){
        if(pFileDesc[i].bUsed==FALSE){
            return i;
        }
    }
}

int FindOffsetBlock(int legnth){
    return legnth/BLOCK_SIZE;
}
int WhereNewDirOrder(DirEntry * direntry,char * szDirName){
    if(FindDir(direntry,szDirName)!=-1) return FAILED;//이미 있다면
    for(int i=0; i<NUM_OF_DIRECT_BLOCK_PTR; i++){
        if(sizeof(direntry[i].name) <= 1){//비어있는곳 발견
            return i;//해당위치 반환
        }
    }
    return -1;
}
int FindDirTable(DirEntry * direntry, char * targetName){
    for(int i=0; i<NUM_OF_DIRECT_BLOCK_PTR; i++){//중복디렉토리 체크
        if(strcmp(direntry[i].name,targetName)){//원하는 이름을 발견하면
            return i;//해당 인덱스 반환
        }
    }
    return FAILED;
}
