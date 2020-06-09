#include "Headers.h"

PathInfo PathFinder(const char *szFileName) {
    if(DEBUGGING) printf("[+] pathFinder %s\n",szFileName);

    PathInfo pathInfo;
    char parsing[120];
    strcpy(parsing, szFileName);
    /**********시작 초기화정보*************/
    TableIndex tableInfo;
    int ptrInodeNum = 0;
    int ptrBlock = 7;
    /********루트디렉토리 가져오기*******/
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    Inode *pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(0,pInode);
    DirEntry *dirEntry;
    /************************************/
    char *ptr = strtok(parsing, "/");
    strcpy(pathInfo.name,ptr);
    pathInfo.pInodeNum = ptrInodeNum;

    while (ptr != NULL) // 자른 문자열이 나오지 않을 때까지 반복
    {
        tableInfo = FindDirTable(ptrInodeNum, ptr); //디렉토리에서 ptr찾
        if (tableInfo.pi == FAILED) {
            // printf("%s를 찾지 못함 failed\n",ptr);
            strcpy(pathInfo.name, ptr); //이름 복사
            break;                      //못 찾았으면 break
        }
        //이미 만들얼 져 있으니까 당연히 home을 발견했음
        ptrBlock = pInode->dirBlockPtr[tableInfo.pi]; // ptr을 지닌 block번호
        DevReadBlock(ptrBlock, pBuf);
        dirEntry = (DirEntry *)pBuf;

        ptrInodeNum = dirEntry[tableInfo.ti].inodeNum; // ptr의 inode번호
        strcpy(pathInfo.name,dirEntry[tableInfo.ti].name);//ptr의 이름
        pathInfo.pInodeNum = ptrInodeNum;//반환정보
        // printf(" %d %d %d \n",tableInfo.pi, tableInfo.ti,ptrInodeNum);
        GetInode(ptrInodeNum, pInode);//해당 inode 가져오기

        ptr = strtok(NULL, "/"); // 다음 ptr
    }
    free(pBuf);
    free(pInode);

    ptr = strtok(NULL, "/");
    if (ptr != NULL) { //폴더를 찾지 못했는데, 그 다음 token이 있었다면 Failed
        // printf("폴더를 찾지 못했는데, 다음 토큰이 있었음\n");
        pathInfo.pInodeNum = FAILED;
        return pathInfo;
    }
    pathInfo.pInodeNum = ptrInodeNum;
    // printf("return name %s , inode : %d \n",pathInfo.name, pathInfo.pInodeNum);
    if(DEBUGGING) printf("[-] pathFinder\n");
    return pathInfo; //생성파일의 상위디렉토리 블럭번호 반환
}

TableIndex FindDirTable(int in,const char *targetName) {
    if(DEBUGGING) printf("[+] FindDirTable %s\n",targetName);
    // printf("@@####$$$ ?_ ) inode : %d . name : %s \n",in, targetName);
    TableIndex tdx = {FAILED, FAILED};
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    Inode *pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(in, pInode);
    // printf("find dir table : allock (%d) 1 (%d) 2(%d) 3(%d) ",pInode->allocBlocks, 
    // pInode->dirBlockPtr[0],pInode->dirBlockPtr[1],pInode->dirBlockPtr[2]);
    int cnt=0;
    int allockNum = pInode->allocBlocks;
    if(allockNum<0 || allockNum > NUM_OF_DIRECT_BLOCK_PTR) {
        // printf("비 정상적인 allock Num 입니다. %d \n",allockNum);
        allockNum=0;
    }

    for (int i = 0; i < allockNum; i++) { //몇 번째 블록
        DevReadBlock(pInode->dirBlockPtr[i], pBuf);
        DirEntry *direntry = (DirEntry *)pBuf;
        for (int j = 0; j < NUM_OF_DIRENT_PER_BLOCK; j++) { //몇 번째 위치
            if(strcmp(direntry[j].name,"no-entry")==0) {
                cnt++;
            }
            if (strcmp(direntry[j].name, targetName) == 0) { //해당하는 이름을 찾았다면
                tdx.pi = i;
                tdx.ti = j;
                free(pBuf);
                free(pInode);
                // printf("[-] FindDirTable %s 발견%d %d\n",targetName,i,j);
                return tdx;
            }
        }
    }
    // printf("%d inode에 저장된 allock blocks : %d , %d %d\n",in,pInode->allocBlocks,tdx.pi, tdx.ti);

    if (allockNum!=0 && cnt==0 && pInode->allocBlocks < NUM_OF_DIRECT_BLOCK_PTR) {
        // printf("파일 보유한도 초과 블록확장 %d \n",in);
        AllocNewBlock(in); //빈칸이 하나도 없고 해당하는 이름을 찾지 못했다면
        free(pBuf);
        free(pInode);
        return FindDirTable(in,targetName);//칸을 늘리고 다시 탐색
    }

    free(pBuf);
    free(pInode);
    if(DEBUGGING) printf("[-] FindDirTable\n");
    return tdx;
}
int AllocNewBlock(int inode) {
    /***블럭 추가 함수**/
    if(DEBUGGING) printf("[+] AllocNewBlock\n");
    int bn = GetFreeBlockNum();
    int in = GetFreeInodeNum();
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    Inode *pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(inode, pInode);//원본 아이노드

    /********생성할 블럭*******/
    DevReadBlock(bn, pBuf);
    DirEntry * direntry = (DirEntry *) pBuf;
    DirEntryInit(direntry);
    DevWriteBlock(bn, pBuf);

    /********아이노드에 블럭 추가**********/
    pInode->dirBlockPtr[pInode->allocBlocks] = bn;
    pInode->allocBlocks++;
    pInode->size = (pInode->allocBlocks) * BLOCK_SIZE;
    PutInode(inode,pInode);
    /**********************************/
    SetInodeBytemap(in);
    SetBlockBytemap(bn);
    
    /********디스크정보 갱신**********/
    pFileSysInfo->numAllocBlocks++;
    pFileSysInfo->numFreeBlocks--;
    DevWriteBlock(FILESYS_INFO_BLOCK,(char *)pFileSysInfo);

    free(pBuf);
    free(pInode);
    if(DEBUGGING) printf("[-] AllocNewBlock\n");
}
void printDirEntry(int in){
    char * pBuf = (char *)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    DirEntry * direntry;
    GetInode(in, pInode);
    if(pInode->allocBlocks > NUM_OF_DIRECT_BLOCK_PTR){
        free(pInode);
        return;
    }
    printf("----------------------------------\n");
    for(int i=0;i <pInode->allocBlocks; i++){

        DevReadBlock(pInode->dirBlockPtr[i],pBuf);
        direntry = (DirEntry *)pBuf;
        for(int j=0; j<NUM_OF_DIRENT_PER_BLOCK; j++){
            printf("direntry %d : %s \t %d\n",i, direntry[j].name, direntry[j].inodeNum);
        }
    }
    printf("----------------------------------\n");
    
    free(pInode);
}
void BlockInit(char *pBuf) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        pBuf[i] = 0;
    }
}
void DirEntryInit(DirEntry *direntry) {
    for (int i = 0; i < NUM_OF_DIRENT_PER_BLOCK; i++) {
        strcpy(direntry[i].name, "no-entry");
        direntry[i].inodeNum = NONE;
    }
}
void print_pSysInfo() {
    printf("numAllockBlocks : %d, numFreeBlocks : %d, numAllockInodes : %d\n",
           pFileSysInfo->numAllocBlocks, pFileSysInfo->numFreeBlocks,
           pFileSysInfo->numAllocInodes);
}

int UsingInode(int in){
    char * pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,pBuf);
    if(pBuf[in]==1)
        return TRUE;
    else
        return FALSE;
}
int IsEmptyDir(int in){
    /*비어있다면 TRUE반환*/
    Inode * rInode = (Inode *)malloc(sizeof(Inode));//remove Inode
    GetInode(in,rInode);
    if(rInode->allocBlocks!=1){
        free(rInode);
        return FAILED;
    }
    char * pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(rInode->dirBlockPtr[0],pBuf);
    DirEntry * direntry = (DirEntry *)pBuf;
    //일단 하나여야지 비어있는 블록이라고 판단.
    //왜냐면 기본폴더 두개 "."랑 ".."가 0에 할당되어있기 때문에
    for(int i=2; i<NUM_OF_DIRENT_PER_BLOCK; i++){//32개 둘러보기
        //뭐가 들어있으면 Failed
        if(strcmp(direntry[i].name,"no-entry")!=0) return FAILED;
    }
    return TRUE;//걸린게 없으면 성공
}

int UsingBlock(int bn){
    char * pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(BLOCK_BYTEMAP_BLOCK_NUM,pBuf);
    if(pBuf[bn]==1)
        return TRUE;
    else
        return FALSE;
}

int FindDescriptorTable(){
    /*사용가능한 descriptorTable인덱스 반환*/
    for(int i=0; i<MAX_FD_ENTRY_MAX; i++){
        if(pFileDesc[i].bUsed==FALSE){
            return i;
        }
    }
}
