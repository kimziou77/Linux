#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "fs.h"
#include "Headers.h"
void FileSysInit(){
    if(DEBUGGING) printf("[+] FileSysInit\n");

    /*********FileDescripter 초기화*********/
    for(int i=0; i<MAX_FD_ENTRY_MAX; i++){
        pFileDesc[i].bUsed = FALSE;
        pFileDesc[i].pOpenFile=NULL;
    }
    /***********Disk 초기화************/
    //(1) Block 크기의 메모리 할당하고 0으로 채운다
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    memset(pBuf,0,BLOCK_SIZE);
    //(2) 메모리를 Block 0부터 6까지 저장한다.
    for(int i=0; i<7; i++)
        DevWriteBlock(i,pBuf);
    //fileSysInfo, inodeBytemap, blockBytemap, inodeList를 0으로 초기화    
    DevWriteBlock(BLOCK_BYTEMAP_BLOCK_NUM,pBuf);

    for(int i=0; i<7; i++)
        SetBlockBytemap(i);//0~6번블록 사용중으로 표시한다.
    free(pBuf);//내 기준 ..write를 해주었으니 임시로 만든 Buf는 지워준다..
    

    /***********루트 디렉토리 생성************/
    int bn = GetFreeBlockNum();
    int in = GetFreeInodeNum();
    pBuf = (char*)malloc(BLOCK_SIZE);
    DirEntry* rootDirEntry = (DirEntry *)pBuf;
    DirEntryInit(rootDirEntry);
    //
    strcpy(rootDirEntry[0].name,".");
    rootDirEntry[0].inodeNum = in;
    //
    DevWriteBlock(bn,pBuf);// root정보 디스크 저장
    free(pBuf);

    /*************fileSysInfo*******************/
    pBuf = (char *)malloc(BLOCK_SIZE);
    FileSysInfo * fileSysInfo = (FileSysInfo *)pBuf;
    fileSysInfo->blocks = 512;
    fileSysInfo->rootInodeNum = 0;
    fileSysInfo->diskCapacity = FS_DISK_CAPACITY;
    fileSysInfo->numAllocBlocks = 0;
    fileSysInfo->numFreeBlocks = 512 - 7;
    fileSysInfo->numAllocInodes = 0;
    fileSysInfo->inodeBytemapBlock = INODE_BYTEMAP_BLOCK_NUM;//1
    fileSysInfo->blockBytemapBlock = BLOCK_BYTEMAP_BLOCK_NUM;//2
    fileSysInfo->inodeListBlock = INODELIST_BLOCK_FIRST;//3
    fileSysInfo->dataRegionBlock = 7;
    //초기값설정
    pFileSysInfo = fileSysInfo;
    //변경
    pFileSysInfo->numAllocBlocks++;
    pFileSysInfo->numAllocInodes++;
    pFileSysInfo->numFreeBlocks--;
    DevWriteBlock(FILESYS_INFO_BLOCK,pBuf);//FILESYS 정보 디스크 저장

    /*******************************************/
    SetBlockBytemap(bn);
    SetInodeBytemap(in);

    /**************RootInode초기화**************/
    Inode * rootInode = (Inode *)malloc(sizeof(Inode));
    GetInode(0,rootInode);
    rootInode->allocBlocks = 1;//"."
    rootInode->type = FILE_TYPE_DIR;
    rootInode->size = (rootInode->allocBlocks)*(BLOCK_SIZE);
    rootInode->dirBlockPtr[0] = bn;
    PutInode(0,rootInode);

    if(DEBUGGING) printf("[-] FileSysInit\n");
}

void SetInodeBytemap(int inodeno)
{
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(1,pBuf);
    pBuf[inodeno]=1;
    //pFileSysInfo->numAllocInodes++;
    DevWriteBlock(1,pBuf);
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
    memset(pBuf,0,BLOCK_SIZE);
    int blkno = WhereIsInodeBlock(inodeno);
    int order = WhereIsInodeOrder(inodeno);
    DevReadBlock(blkno,pBuf);//원본데이터를 읽어들인다.
    Inode * Buf = (Inode *)pBuf;//pBuf는 현재 Inode[0] 주소를 지님
    Buf[order] = *pInode;

    DevWriteBlock(blkno,(char *)Buf);
    free(pBuf);
}

void GetInode(int inodeno, Inode* pInode)
{
    /*inodeno을 디스크에서 읽어 pInode에 저장한다 : Write필요 x*/
    char *pBuf = (char *)malloc(BLOCK_SIZE);

    int blkno = WhereIsInodeBlock(inodeno);
    int order = WhereIsInodeOrder(inodeno);
    DevReadBlock(blkno,pBuf); //inode가 존재하는 블럭을 Read한다.
    Inode * Buf = (Inode *)pBuf;//Inode크기만큼 증가한다.

    *pInode = Buf[order];

    DevWriteBlock(blkno,(char *)Buf);
    free(pBuf);
}

int GetFreeInodeNum(void)
{
    //if(DEBUGGING) printf("[+] GetFreeInodeNum\n");
    char * pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(1,pBuf); //InodeBytemap Block (1)
    for(int i=0; i<BLOCK_SIZE;i++){
        if(pBuf[i]==0){
            free(pBuf);
            //if(DEBUGGING) printf("[-] GetFreeInodeNum\n");
            return i;
        }
    }
    //if(DEBUGGING) printf("[-] GetFreeInodeNum\n");
}

int GetFreeBlockNum(void)
{
    //if(DEBUGGING) printf("[+] GetFreeBlockNum\n");
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(BLOCK_BYTEMAP_BLOCK_NUM,pBuf);
    for(int i=7; i<BLOCK_SIZE; i++){//7번부터 DataRegion이므로, 7번부터 탐색
        if(pBuf[i]==0){
            free(pBuf);
      //      if(DEBUGGING) printf("[-] GetFreeBlockNum\n");
            return i;
        }
    }
    //if(DEBUGGING) printf("[-] GetFreeBlockNum\n");
}
int WhereIsInodeBlock(int inodeno){
    /*해당 inode가 몇 번째 블록에 있는지 반환*/
    int no = inodeno/NUM_OF_INODE_PER_BLOCK + INODELIST_BLOCK_FIRST;
    //printf("whereis Inode: %d\n",no);
    return no;
}
int WhereIsInodeOrder(int inodeno){
    /*해당 inode가 몇 번째 순서에 있는지 반환*/
    return inodeno % NUM_OF_INODE_PER_BLOCK;
}
