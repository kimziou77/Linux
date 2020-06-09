#include "Headers.h"
int		MakeDir(const char* szDirName)
{
    int pBlockNum = pathFinder(szDirName);
    if(pBlockNum == -1) return FAILED;

    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    DevReadBlock(pBlockNum,pBuf);
    DirEntry* dirEntry = (DirEntry *)pBuf;
    GetInode(dirEntry[0].inodeNum,pInode);

    int bn = GetFreeBlockNum();
    int in = GetFreeInodeNum();

    char * nName = NameFinder(szDirName);
    if(nName == NULL) return FAILED;

    int nd = WhereNewDirOrder(dirEntry,nName);
    if(nd==FAILED) {
        free(pBuf);
        free(pInode);
        return FAILED;
    }

    strcpy(dirEntry[nd].name,nName);
    dirEntry[nd].inodeNum = in;
    
    pInode->allocBlocks++;
    pInode->size=(pInode->allocBlocks)*BLOCK_SIZE;
    
    DevWriteBlock(pBlockNum,pBuf);
    free(pBuf);

    /* 2 szName Directory 내부 폴더 초기화*/
    pBuf = (char *)malloc(BLOCK_SIZE);
    dirEntry = (DirEntry *)pBuf;
    for(int i=0; i<NUM_OF_DIRECT_BLOCK_PTR; i++){
        strcpy(dirEntry[i].name,"no-entry");
    }
    strcpy(dirEntry[0].name,".");
    dirEntry[0].inodeNum = in;
    strcpy(dirEntry[1].name,"..");
    dirEntry[1].inodeNum = 0;
    
    DevWriteBlock(bn,pBuf);
    free(pBuf);

    /* 2-2 szName Directory iNode 초기화*/
    GetInode(in,pInode);
    pInode->allocBlocks = 2;
    pInode->size = (pInode->allocBlocks)*(BLOCK_SIZE);
    pInode->type = FILE_TYPE_DIR;
    if(DEBUGGING) {
        printf("%s의 block : %d , inodeNum : %d\n",bn , in);
    }
    pInode->dirBlockPtr[0] = bn;//0번은 "."  자기자신 bn(8)
    pInode->dirBlockPtr[1] = pBlockNum;//1번은 ".." 상위폴더 Parent Block Num
    PutInode(in,pInode);
    free(pInode);

    //bytemap 업데이트
    SetBlockBytemap(bn);//새로 할당해준 블록
    SetInodeBytemap(in);//새로 할당해준 inode

    //FileSysInfo 변경
    pFileSysInfo->numAllocBlocks++;
    pFileSysInfo->numFreeBlocks--;
    pFileSysInfo->numAllocInodes++;
    DevWriteBlock(0,(char*)pFileSysInfo);

    return SUCCESS;
}
