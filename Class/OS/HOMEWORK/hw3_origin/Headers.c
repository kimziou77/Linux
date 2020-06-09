#include "Headers.h"

int pathFinder(const char * szFileName){
    if(DEBUGGING) printf("[+] pathFinder \n");
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(0,pInode);//절대경로 -> 루트부터 시작
    DevReadBlock(7,pBuf);
    DirEntry* dirEntry = (DirEntry *)pBuf;

    int ptrTableIndex=0;      //ptr에 대한 DirTable인덱스
    int ptrInodeNum=0;        //ptr에 대한 inode
    int ptrBlock=7;           //ptr블럭

    char parsing[100];//input이 100이 안넘는다고 가정...
	strcpy(parsing, szFileName);
	char* ptr = strtok(parsing, "/");
	while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
	{
        ptrTableIndex = FindDirTable(dirEntry,ptr);//디렉토리에서 ptr찾기
        if(ptrTableIndex==FAILED) break;   //못 찾았으면 break
        
        ptrInodeNum = dirEntry[ptrTableIndex].inodeNum;
        if(DEBUGGING) printf("inode num : %d\n",dirEntry[ptrTableIndex].inodeNum);
        GetInode(ptrInodeNum, pInode);                   //inode를 가지고 온다.
        ptrBlock = pInode -> dirBlockPtr[0];           //ptr의 자신의 블록번호를 가지고 온다.
        DevReadBlock(ptrBlock,pBuf);                    //ptr블럭을 가지고 온다.
        dirEntry = (DirEntry *)pBuf;
		ptr = strtok(NULL, "/");                        // 다음 ptr
	}
    free(pBuf);

    ptr = strtok(NULL,"/"); 
    if(ptr != NULL) {
        if(DEBUGGING) printf("[-] pathFinder 존재하지 않는 폴더 존재 \n");
        return FAILED;//그 다음 token이 있었다면 Failed
    }
    if(DEBUGGING) {
        printf("반환되는 부모 정보 inode : %d ptrBlock :%d\n",ptrInodeNum,ptrBlock);
    }
    if(DEBUGGING) printf("[-] pathFinder 탐색완료 부모블럭 %d 반환  \n",ptrBlock);
    return ptrBlock;//생성파일의 상위디렉토리 블럭번호 반환
}

int pathFinder_n(const char * szFileName){
    if(DEBUGGING) printf("[+] pathFinder_n :%s\n",szFileName);
    /*szFileName의 마지막경로에 대한 Inode를 반환*/
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(0,pInode);//절대경로 -> 루트부터 시작
    DevReadBlock(7,pBuf);
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
        if(ptrTableIndex==FAILED){
            if(DEBUGGING) printf("[-] pathFinder_n \n");
            return FAILED;   //못 찾았으면 failed...
        }
        ptrInodeNum = dirEntry[ptrTableIndex].inodeNum;
        GetInode(ptrInodeNum,pInode);                   //inode를 가지고 온다.
        ptrBlock = pInode -> dirBlockPtr[0];            //ptr의 자신의 블록번호를 가지고 온다.
        DevReadBlock(ptrBlock,pBuf);                    //ptr블럭을 가지고 온다.
        dirEntry = (DirEntry *)pBuf;
		ptr = strtok(NULL, "/");                        // 다음 ptr
	}
    free(pBuf);
    if(DEBUGGING) {
        printf("inode : %d ptrBlock :%d\n",ptrInodeNum,ptrBlock);
    }
    if(DEBUGGING) printf("[-] pathFinder_n \n");
    return ptrInodeNum;
}
char* NameFinder(const char * szFileName){
    if(DEBUGGING) printf("[+] NameFinder :%s\n",szFileName);
    /*szFileName의 마지막경로에 대한 Inode를 반환*/
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(0,pInode);//절대경로 -> 루트부터 시작
    DevReadBlock(7,pBuf);

    DirEntry* dirEntry = (DirEntry *)pBuf;
    int ptrTableIndex = 0;      //ptr에 대한 DirTable인덱스
    int ptrInodeNum = 1;        //ptr에 대한 inode
    int ptrBlock = 7;           //ptr블럭

    char parsing[100];//input이 100이 안넘는다고 가정...
	strcpy(parsing, szFileName);
	char* ptr = strtok(parsing, "/");//TODO: 메모리가...어떻게 되려나..
    char * name = (char *)malloc(sizeof(char) *MAX_NAME_LEN);
    strcpy(name, ptr);
	while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
	{
        ptrTableIndex = FindDirTable(dirEntry,ptr);//디렉토리에서 ptr찾기
        if(ptrTableIndex==FAILED){
            strcpy(name, ptr);
            // name = ptr;
            break;//일단 못찾았으면 break 하고
        }

        ptrInodeNum = dirEntry[ptrTableIndex].inodeNum;
        GetInode(ptrInodeNum,pInode);                   //inode를 가지고 온다.
        ptrBlock = pInode -> dirBlockPtr[0];            //ptr의 자신의 블록번호를 가지고 온다.
        DevReadBlock(ptrBlock,pBuf);                    //ptr블럭을 가지고 온다.
        dirEntry = (DirEntry *)pBuf;
        strcpy(name, ptr);
        // name = ptr;
		ptr = strtok(NULL, "/");                        // 다음 ptr
	}
    free(pBuf);
    ptr = strtok(NULL, "/");                        // 다음 ptr

    if(ptr != NULL) {//다음토큰이 있었다면
        if(DEBUGGING) printf("[-] NameFinder :상위 폴더가 존재하지 않음.\n");
        return NULL;//그 다음 token이 있었다면 Failed
    }
    else{//다음 토큰이 없었다면 마지막폴더였음.
        if(DEBUGGING) printf("[-] NameFinder : name(%s)\n",name);
        return name;
    }

    
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
int WhereNewDirOrder(int in, char * szDirName){
    if(DEBUGGING) printf("[+] WhereNewDirOrder : %s\n",szDirName);
    // printDirEntry(direntry);

    if(FindDirTable(in,szDirName)!=-1) {
        if(DEBUGGING) printf("[-] WhereNewDirOrder 이미 존재 \n");
        return FAILED;
    }

    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    memset(pInode,0,sizeof(Inode));
    GetInode(in, pInode);
    char * pBuf = (char *)malloc(sizeof(BLOCK_SIZE));

    for(int i=0; i < pInode->allocBlocks ; i++){
        DevReadBlock(pInode ->dirBlockPtr[i],pBuf);
        DirEntry * direntry = (DirEntry *)pBuf;
        for(int j=0; j<NUM_OF_DIRENT_PER_BLOCK; j++){
            if(strcmp(direntry[i].name,"no-entry")==0){//비어있는곳 발견
                free(pBuf);
                free(pInode);
                if(DEBUGGING) printf("[-] WhereNewDirOrder 빈곳발견  : %d \n",i);
                return i;//해당위치 반환
            }    
        }
    }
    free(pBuf);
    free(pInode);
    if(DEBUGGING) printf("[-] WhereNewDirOrder 비어있는공간없음 \n");
    return -1;
}
void printDirEntry(DirEntry * direntry){
    printf("----------------------------------\n");
    for(int i=0; i<NUM_OF_DIRENT_PER_BLOCK; i++){
        printf("direntry %d : %s \t %d\n",i, direntry[i].name,direntry[i].inodeNum);
    }
    printf("----------------------------------\n");
}

int FindDirTable(int in, const char * targetName){
    if(DEBUGGING) printf("[+] FindDirTable :%s \n",targetName);

    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    memset(pInode,0,sizeof(Inode));
    GetInode(in, pInode);
    char * pBuf = (char *)malloc(sizeof(BLOCK_SIZE));
    // if(DEBUGGING) printf("@\n");
    for(int i=0; i < pInode->allocBlocks ; i++){
        DevReadBlock(pInode ->dirBlockPtr[i],pBuf);
        DirEntry * direntry = (DirEntry *)pBuf;
        // if(DEBUGGING) printf("@@@@@###s\n");
        for(int j=0; j<NUM_OF_DIRENT_PER_BLOCK; j++){
            if(strcmp(direntry[i].name,targetName)==0){//원하는 이름을 발견하면
                if(DEBUGGING) printf("[-] FindDirTable %s 이름 발견  %d  // %d\n",
                    direntry[i].name,direntry[i].inodeNum,i);
                free(pBuf);
                free(pInode);
                return i;//해당 인덱스 반환
            }
        }
    }
    // if(DEBUGGING) printf("@@@\n");
    // free(pBuf);
    // free(pInode);
    if(DEBUGGING) printf("[-] FindDirTable %s 이름 없음  \n",targetName);
    return FAILED;
}
void print_pSysInfo(){
    printf("numAllockBlocks : %d, numFreeBlocks : %d, numAllockInodes : %d\n",
            pFileSysInfo->numAllocBlocks ,pFileSysInfo->numFreeBlocks,pFileSysInfo->numAllocInodes );

}
void DirEntryInit(DirEntry * direntry){
    for(int i=1; i<NUM_OF_DIRENT_PER_BLOCK; i++){
        strcpy(direntry[i].name,"no-entry");
        direntry[i].inodeNum = NONE;
    }
}

void BlockInit(char * pBuf){
    memset(pBuf,0,BLOCK_SIZE);
}
