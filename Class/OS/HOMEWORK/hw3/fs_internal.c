#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "fs.h"
void FileSysInit(){
    /*
    (1) Block 크기의 메모리 할당하고 0으로 채운다
    (2) DevWriteBlock 함수를 통해 메모리를 Block 0부터 6까지 저장한다.
    */
   
    char * pBuf = (char*)malloc(BLOCK_SIZE);//Block크기 메모리 할당
    //FileSysInfo - Block 0
    pFileSysInfo->blocks = 512;// 디스크에 저장된 전체 블록 개수
    pFileSysInfo->rootInodeNum;// 루트 inode의 번호
    pFileSysInfo->diskCapacity = FS_DISK_CAPACITY;// 디스크 용량 (Byte 단위)
    pFileSysInfo->numAllocBlocks;// 파일 또는 디렉토리에 할당된 블록 개수
    pFileSysInfo->numFreeBlocks;//  할당되지 않은 블록 개수
    pFileSysInfo->numAllocInodes;//할당된 inode 개수 
    pFileSysInfo->blockBytemapBlock = BLOCK_BYTEMAP_BLOCK_NUM;
    pFileSysInfo->inodeBytemapBlock = INODE_BYTEMAP_BLOCK_NUM;
    pFileSysInfo->inodeListBlock = INODELIST_BLOCK_FIRST;
    pFileSysInfo->dataRegionBlock = INODELIST_BLOCK_FIRST + INODELIST_BLOCKS;
    pBuf = (char*)pFileSysInfo;
    //사이즈가 안맞긴하는데 이렇게 해도 되는걸까..
    //포인터니까 같은 4bytes긴한데
    DevWriteBlock(FILESYS_INFO_BLOCK,pBuf);//0

    //Inode ByteMap - Block 1 //byte[64]- free(0),allocated(1);
    //TODO: INode byteMap은 64byte인데 이걸 어떻게.... 생성하지.... 남는 부분을 어떻게..?

    //Block ByteMap - Block 2 //byte[512] - 총 블럭크기&&블럭개수
    memset(pBuf,0,BLOCK_SIZE);//0으로 초기화
    DevWriteBlock(BLOCK_BYTEMAP_BLOCK_NUM,pBuf);//pBuf에 512만큼의 크기 할당해주기
    for(int i=0; i<7; i++)
        SetBlockBytemap(i);//0~6번블록 사용중으로 표시한다.
    

    //InodeList - Block 3~6 (4개) //Block당 Inode(32bytes) 16개, 총 4개블럭
    //DataRegion ......
    //for
    Inode * tmpInode = (Inode *)malloc(sizeof(Inode));
    tmpInode ->allocBlocks = INODELIST_BLOCK_FIRST;//+i
    tmpInode ->size;
    tmpInode ->type;
    tmpInode ->dirBlockPtr;

    //TODO: 그냥 모두 Block사이즈만큼 넣으면 안되나??? Inode를 생성해서 넣어주어야 하나?
    //루트를 만들어서 루트에 대한 Inode를 넣기

    /******************************************/

//     char * pBuf;
//     Inode * pInode;
//     DevWriteBlock(0,pBuf);
//    //malloc();
//     GetInode(0,pInode);
//     SetInode(0,pInode);
    free(pBuf);    
}

void SetInodeBytemap(int inodeno)
{
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,pBuf);
    pBuf[inodeno]=1;
    //pFileSysInfo->numAllocInodes++;
    DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM,pBuf);
    free(pBuf);
}

void ResetInodeBytemap(int inodeno)
{
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,pBuf);
    pBuf[inodeno]=0;
    //pFileSysInfo->numAllocInodes--;
    DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM,pBuf);
    free(pBuf);
}

void SetBlockBytemap(int blkno)
{
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(BLOCK_BYTEMAP_BLOCK_NUM,pBuf);
    pBuf[blkno]=1;
    //pFileSysInfo->numAllocBlocks++;
    DevWriteBlock(BLOCK_BYTEMAP_BLOCK_NUM,pBuf);
    free(pBuf);
}

void ResetBlockBytemap(int blkno)
{
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(BLOCK_BYTEMAP_BLOCK_NUM,pBuf);
    pBuf[blkno]=0;
    //pFileSysInfo->numAllocBlocks--;
    DevWriteBlock(BLOCK_BYTEMAP_BLOCK_NUM,pBuf);
    free(pBuf);
}


void PutInode(int inodeno, Inode* pInode)
{
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    int blkno = WhereIsInodeBlock(inodeno);
    int order = WhereIsInodeOrder(inodeno);
    DevReadBlock(blkno,pBuf);

    Inode ** Buf = (Inode **)pBuf;
    Buf[order] = pInode;
    //TODO: 복사를 해야하는데 대입을 해버리면 되나..? 안될거같은데
    //깊은복사를 해주어야 할 듯...

    free(pBuf);
}


void GetInode(int inodeno, Inode* pInode)
{
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    int blkno = WhereIsInodeBlock(inodeno);
    int order = WhereIsInodeOrder(inodeno);
    DevReadBlock(blkno,pBuf);
    Inode ** Buf = (Inode **)pBuf;//Inode* 의 주소를 가지고 있다.
    pInode = Buf[order];//인덱스로 접근하면 Inode *가 나온다.
    //TODO: 복사를 해야하는데 대입을 해버리면 되나..? 안될거같은데
    //깊은복사를 해주어야 할 듯...

    free(pBuf);
    //pBuf를 free를 안하게 되면 남은 공간들은 접근할 수 없어서 메모리 낭비 아닌가?
}


int GetFreeInodeNum(void)
{
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,pBuf);
    for(int i=0; i<BLOCK_SIZE;i++){
        if(pBuf[i]==1){
            free(pBuf);
            return i;
        }
    }
}


int GetFreeBlockNum(void)
{
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(BLOCK_BYTEMAP_BLOCK_NUM,pBuf);
    for(int i=0; i<BLOCK_SIZE;i++){
        if(pBuf[i]==1){
            free(pBuf);
            return i;
        }
    }
}

int WhereIsInodeBlock(int inodeno){
    return inodeno / NUM_OF_INODE_PER_BLOCK;
}
int WhereIsInodeOrder(int inodeno){
    return inodeno % NUM_OF_INODE_PER_BLOCK;
}
