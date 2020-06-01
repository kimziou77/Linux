#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "fs.h"

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
    for(int i=7; i<BLOCK_SIZE;i++){//7번부터 DataRegion이므로, 7번부터 탐색
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
