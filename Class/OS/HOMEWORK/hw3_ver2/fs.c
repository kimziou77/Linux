#include "fs.h"
#include "Headers.h"
int CreateFile(const char *szFileName) {
    if (DEBUGGING) printf("[+] CreateFile\n");
    
    PathInfo pi = PathFinder(szFileName);
    TableIndex Ti = FindDirTable(pi.pInodeNum, "no-entry");

    //상위 디렉토리 inode불러오기
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    Inode *pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(pi.pInodeNum, pInode);
    DevReadBlock(pInode->dirBlockPtr[Ti.pi], pBuf); //자식을 넣을 블록
    DirEntry *dirEntry = (DirEntry *)pBuf;

    int in = GetFreeInodeNum();

    //부모디렉토리 블럭정보 변경
    strcpy(dirEntry[Ti.ti].name, pi.name);
    dirEntry[Ti.ti].inodeNum = in;
    DevWriteBlock(pInode->dirBlockPtr[Ti.pi], pBuf); //수정정보 디스크에 저장
    // free(pBuf);

    /* 2-2 szName Directory iNode 초기화*/
    GetInode(in, pInode);    //자기자신의 iNode 불러오기
    pInode->allocBlocks = 0; // 아직 아무것도 쓰지 않았음..
    pInode->size = 0;
    pInode->type = FILE_TYPE_FILE;
    pInode->dirBlockPtr[0];
    PutInode(in, pInode);
    free(pInode);

    // bytemap 업데이트
    SetInodeBytemap(in); //새로 할당해준 inode
    int nfd = FindDescriptorTable();
    File * file = (File *)malloc(sizeof(File));
    file->fileOffset = 0;
    file->inodeNum = in;
    pFileDesc[nfd].pOpenFile = file;

    // FileSysInfo 변경
    pFileSysInfo->numAllocInodes++;
    DevWriteBlock(FILESYS_INFO_BLOCK, (char *)pFileSysInfo);

    if (DEBUGGING) printf("[-] CreateFile\n");
    return nfd;
}

int OpenFile(const char *szFileName) {}

int WriteFile(int fileDesc, char *pBuffer, int length) {
    //Block 단위로 읽고 쓰나보다???
    /*파일 불러오기*/
    File * file = pFileDesc[fileDesc].pOpenFile;
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(file->inodeNum,pInode);
    
    /*inode에 할당한 블록 설정해주기*/
    int bn = GetFreeBlockNum();
    int ptrNum = file->fileOffset/BLOCK_SIZE;
    pInode->dirBlockPtr[ptrNum] = bn;
    file->fileOffset += BLOCK_SIZE;
    PutInode(file->inodeNum,pInode);
    
    /*데이터 복사*/
    char * pBuf = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(bn,pBuf);
    for(int i=0; i<BLOCK_SIZE; i++){
        pBuf[i] = pBuffer[i];
    }
    DevWriteBlock(bn,pBuf);
    free(pBuf);
}

int ReadFile(int fileDesc, char *pBuffer, int length) {}

int CloseFile(int fileDesc) {}

int RemoveFile(const char *szFileName) {

}

int MakeDir(const char *szDirName) {
    if (DEBUGGING) printf("[+] MakeDir\n");
    
    PathInfo pi = PathFinder(szDirName);
    if (DEBUGGING) printf("name : %s \n", pi.name);

    TableIndex Ti = FindDirTable(pi.pInodeNum, "no-entry");
    //상위 디렉토리 inode불러오기
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    Inode *pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(pi.pInodeNum, pInode);
    DevReadBlock(pInode->dirBlockPtr[Ti.pi], pBuf); //자식을 넣을 블록
    DirEntry *dirEntry = (DirEntry *)pBuf;

    int bn = GetFreeBlockNum();
    int in = GetFreeInodeNum();
    if (DEBUGGING) printf("생성될 number >> blockNum : %d , InodeNum :%d\n", bn, in);

    //부모디렉토리 블럭정보 변경
    strcpy(dirEntry[Ti.ti].name, pi.name);
    dirEntry[Ti.ti].inodeNum = in;
    DevWriteBlock(pInode->dirBlockPtr[Ti.pi], pBuf); //수정정보 디스크에 저장
    // free(pBuf);

    /* 2 szName Directory 내부 폴더 초기화*/
    // pBuf = (char *)malloc(BLOCK_SIZE);//(1) Block크기의 메모리 할당.
    // dirEntry = (DirEntry *)pBuf;//(2) DirEntry구조체의 배열로 변환
    DirEntryInit(dirEntry);
    strcpy(dirEntry[0].name, "."); //(3) DirEntry[1]의 변수들을 설정함.
    dirEntry[0].inodeNum = in;     //자기 자신의 iNode
    strcpy(dirEntry[1].name, "..");
    dirEntry[1].inodeNum = pi.pInodeNum; // 상위 디렉토리의 iNode
    DevWriteBlock(bn, pBuf); //(4) 해당 directory파일을 디렉토리에 저장한다.
    // free(pBuf);

    /* 2-2 szName Directory iNode 초기화*/
    GetInode(in, pInode);    //자기자신의 iNode 불러오기
    pInode->allocBlocks = 1; // 자기자신에게 할당된 블록 1
    pInode->size = (pInode->allocBlocks) * (BLOCK_SIZE);
    pInode->type = FILE_TYPE_DIR;
    pInode->dirBlockPtr[0] = bn;
    PutInode(in, pInode);
    free(pInode);
    // bytemap 업데이트
    SetBlockBytemap(bn); //새로 할당해준 블록
    SetInodeBytemap(in); //새로 할당해준 inode

    // FileSysInfo 변경
    pFileSysInfo->numAllocBlocks++;
    pFileSysInfo->numFreeBlocks--;
    pFileSysInfo->numAllocInodes++;
    DevWriteBlock(FILESYS_INFO_BLOCK, (char *)pFileSysInfo);
    if (DEBUGGING)
        printf("[-] MakeDir\n");

    return SUCCESS;
}

int RemoveDir(const char *szDirName) {
    if (DEBUGGING) printf("[+] RemoveDir\n");
    //빈 디렉토리가 아니라면 Failed
    char * pBuf = (char *)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));//remove Inode
    PathInfo pi = PathFinder(szDirName);//지울 본인의 inode와 이름을 가지고 옴.
    GetInode(pi.pInodeNum, pInode);

    if(pInode->type != FILE_TYPE_DIR) return FAILED;//폴더 아니면 실패
    if(!IsEmptyDir(pi.pInodeNum)) return FAILED;//비어있지 않으면 실패

    //위에서 실패하지 않았다면 지울 수 있는 디렉토리이다.
    pInode->type=FILE_TYPE_DEV;
    PutInode(pi.pInodeNum,pInode);
    ResetBlockBytemap(pInode->dirBlockPtr[0]);//이제 사용 안하는 블럭이라고 표시해주기
    ResetInodeBytemap(pi.pInodeNum);

    //부모 아이노드 겟
    DevReadBlock(pInode->dirBlockPtr[0],pBuf);
    DirEntry * direntry = (DirEntry *)pBuf;
    int pIn = direntry[1].inodeNum;//부모 Inode number
    TableIndex Ti = FindDirTable(pIn,pi.name);

    GetInode(pIn,pInode);
    DevReadBlock(pInode->dirBlockPtr[Ti.pi],pBuf);
    direntry = (DirEntry *)pBuf;

    /*부모디렉토리에서 지우려는 디렉토리 이름을 지운다.*/
    strcpy(direntry[Ti.ti].name,"no-entry");
    direntry[Ti.ti].inodeNum = NONE;
    int num = 0;
    for(int i=0; i<NUM_OF_DIRENT_PER_BLOCK; i++){
        if(strcmp(direntry[i].name,"no-entry")==0){
            num++;
        }
    }

    if(num==NUM_OF_DIRENT_PER_BLOCK){
        //모두 비어있다면 블럭 삭제
        DirEntryInit(direntry);
        pInode->allocBlocks--;
        pInode->size= (pInode->allocBlocks)*BLOCK_SIZE;
        ResetBlockBytemap(pInode->dirBlockPtr[Ti.pi]);
        pFileSysInfo->numAllocBlocks--;
        pFileSysInfo->numFreeBlocks++;
    }
    DevWriteBlock(pInode->dirBlockPtr[Ti.pi],pBuf);
    PutInode(pIn,pInode);
    /***********************************/

    // FileSysInfo 변경
    pFileSysInfo->numAllocBlocks--;
    pFileSysInfo->numFreeBlocks++;
    pFileSysInfo->numAllocInodes--;
    DevWriteBlock(FILESYS_INFO_BLOCK, (char *)pFileSysInfo);

    if (DEBUGGING) printf("[-] RemoveDir\n");
    return SUCCESS;
}

int EnumerateDirStatus(const char *szDirName, DirEntryInfo *pDirEntry,int dirEntrys) {

    PathInfo pi = PathFinder(szDirName); //자기 자신의 inode

    char *pBuf = (char *)malloc(BLOCK_SIZE);
    Inode *pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(pi.pInodeNum, pInode);
    int num = 0;

    for (int i = 0; i < pInode->allocBlocks; i++) {
        DevReadBlock(pInode->dirBlockPtr[i], pBuf);
        DirEntry* dirEntry = (DirEntry *)pBuf;
        Inode * cInode = (Inode *)malloc(sizeof(Inode));
        for (int j = 0; j < NUM_OF_DIRENT_PER_BLOCK; j++) { //순서대로 쭉 읽기.
            if(num > dirEntrys) {
                free(pBuf);
                free(pInode);
                return dirEntrys;
            }
            
            GetInode(dirEntry[j].inodeNum, cInode);//자식 정보 가지고 오기
            if(cInode->type==FILE_TYPE_DIR || cInode->type == FILE_TYPE_FILE){
                if(strcmp(dirEntry[j].name,"no-entry")==0){
                    continue;
                }
                pDirEntry[num].inodeNum = dirEntry[j].inodeNum;
                strcpy(pDirEntry[num].name, dirEntry[j].name);
                pDirEntry[num].type = cInode->type;
                num++;
            }
        }

    }
    if(num<dirEntrys) return num;
}

void CreateFileSystem() {
    DevCreateDisk();
    OpenFileSystem();
    FileSysInit();
}

void OpenFileSystem() { DevOpenDisk(); }

void CloseFileSystem() { DevCloseDisk(); }

int GetFileStatus(const char *szPathName, FileStatus *pStatus) {
    // if(DEBUGGING) printf("[+] GetFileStatus \n");
    PathInfo pi = PathFinder(szPathName);

    //상위 디렉토리 inode불러오기
    char *pBuf = (char *)malloc(BLOCK_SIZE);
    Inode *pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(pi.pInodeNum, pInode);
    DevReadBlock(pInode->dirBlockPtr[0],pBuf);
    DirEntry *dirEntry = (DirEntry *)pBuf;
    
    //Status초기화
    pStatus->allocBlocks = pInode->allocBlocks;

    for(int i=0; i<NUM_OF_DIRECT_BLOCK_PTR; i++){
        pStatus->dirBlockPtr[i] = pInode->dirBlockPtr[i];
        if(DEBUGGING) printf("%d ",pInode->dirBlockPtr[i]);
    }
    if(DEBUGGING) printf("\n");
    pStatus->size = pInode->size;
    pStatus->type = pInode->type;

    // if(DEBUGGING) printf("[-] GetFileStatus \n");
    return SUCCESS;

}
