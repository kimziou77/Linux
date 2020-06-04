#include "Headers.h"

int		CreateFile(const char* szFileName)
{
   int pBlockNum = pathFinder(szFileName);//부모디렉토리 parsing
    if(pBlockNum == -1) return FAILED;

    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    DevReadBlock(pBlockNum,pBuf);
    DirEntry* dirEntry = (DirEntry *)pBuf;//상위 디렉토리를 불러온다.
    GetInode(dirEntry[0].inodeNum,pInode);

    //생성할파일의 Inode. 디렉토리가 아니므로, 블록은 필요하지 않다.
    int in = GetFreeInodeNum();// FreeInode 1
    int nd = WhereNewDirOrder(dirEntry,szFileName);//상위 폴더에 빈칸을 찾아 할당한다.
    if(nd==FAILED) {
        free(pBuf);
        free(pInode);
        return FAILED;//동일한 이름이 있다면 FAILED
    }
    
    //부모디렉토리 블럭정보 변경
    strcpy(szFileName,dirEntry[nd].name);
    dirEntry[nd].inodeNum = in;
    //부모디렉토리 Inode정보 변경
    pInode->allocBlocks++;
    pInode->size=(pInode->allocBlocks)*BLOCK_SIZE;
    DevWriteBlock(pBlockNum,pBuf);//수정정보 디스크에 저장
    free(pBuf);//쓰고나서 임시 Buf는 삭제...

    /* szName iNode 초기화*/
    GetInode(in,pInode);//자기자신의 iNode 불러오기
    pInode->allocBlocks = 0; //파일이라서 할당된 블록은 없다.
    pInode->size = 0;//(pInode->allocBlocks)*(BLOCK_SIZE)
    pInode->type = FILE_TYPE_FILE;//왜 이게 int형이지? FileType형이 아닌가?
    SetInode(in,pInode);
    free(pInode);

    //bytemap 업데이트
    SetInodeBytemap(in);//새로 할당해준 inode

    //FileSysInfo 변경
    pFileSysInfo->numAllocInodes++;
    DevWriteBlock(FILESYS_INFO_BLOCK,(char*)pFileSysInfo);
    //여기까지 파일을 디렉토리안에 생성함

    /*************open까지 해야됨**************/
    int nFDnum = OpenFile(szFileName);
    return nFDnum;//return file descriptor
}

int		OpenFile(const char* szFileName)
{
    int nInodeNum = pathFinder_n(szFileName);//해당 파일의 Inode획득
    if(nInodeNum == -1) return FAILED;//못찾으면 Failed반환

    /*************fd생성**************/
    //(1) FileDescripter의 빈 entry를 찾는다.
    int nFDnum =FindFileDescripterTable();
    //(2) Malloc으로 File객체를 할당한다.
    File * newFile = malloc(sizeof(File));
    //(3) File객체의 변수들을 설정한다. 
    newFile->inodeNum = nInodeNum;
    newFile->fileOffset = 0;//file 처음 오픈 ->offset을 0으로 해줘야댐
    pFileDesc[nFDnum].bUsed = TRUE;
    pFileDesc[nFDnum].pOpenFile = newFile;

    return nFDnum; // fd 반환
}

int		WriteFile(int fileDesc, char* pBuffer, int length)
{
    int bn = GetFreeBlockNum();//빈 블록을 하나 할당받을것임.
    SetBlockBytemap(bn);

    //(1) fileDesc가 지정하는 inode번호를 획득한다.
    Inode * pInode = malloc(sizeof(Inode));
    File* file = pFileDesc[fileDesc].pOpenFile;
    int fileInode = file->inodeNum;
    GetInode(fileInode,pInode);//(2)해당 inode를 획득한다.

    /*Inode 업데이트*/
    //(3)파일Inode의 directBlockPointer[0]에 블록 bn에 저장한다.
    pInode->dirBlockPtr[0] = bn;//데이터를 저장하기 위해 블록 할당.
    pInode->allocBlocks++;
    pInode->size=(pInode->allocBlocks)*BLOCK_SIZE;
    //(4)Inode를 디스크에 저장한다
    SetInode(fileInode,pInode);

    //(1) Block이 할당되었기 때문에 FileSysInfo를 업데이트 한다.
    pFileSysInfo->numAllocBlocks++;
    pFileSysInfo->numFreeBlocks--;
    DevWriteBlock(FILESYS_INFO_BLOCK,(char *)pFileSysInfo);

    /*실제 데이터 저장*/
    //(2) file object의 fileOffset를 512로 설정함.
    file->fileOffset += length;//다음 접근할 위치를 설정 read/write때마다..
    //(3) 블록 크기의 메모리를 할당해서 복사한다.
    char * BlockMemory = (char *)malloc(BLOCK_SIZE);
    for(int i=0; i<length; i++){
        BlockMemory[i] = pBuffer[i];
    }
    //Buffer에는 디스크에 저장하고자 하는 데이터들이 저장되어있음. abcabc

    //(4) 데이터를 저장한 메모리를 디스크의 bn으로 저장한다.
    DevWriteBlock(bn,BlockMemory);
    free(BlockMemory);

    return length;

}

int		ReadFile(int fileDesc, char* pBuffer, int length)
{
    //TODO: 근데 왜 file Offset을 read크기만큼 증가시키지? write는 그렇다 치더라도
    /***********작성할 파일정보 불러들이기***********/
    File* file = pFileDesc[fileDesc].pOpenFile;
    int fileInodeNum = file->inodeNum;
    int fileOffset = file->fileOffset;
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(fileInodeNum,pInode);

    /**************파일 읽기*************/
    //이 offset 위치부터 length만큼 읽어야한다 - ??
    int lBlock = FindOffsetBlock(length);           //logical Block
    int pBlock = pInode->dirBlockPtr[lBlock];       //Physical Block
    char * readBlock = (char *)malloc(BLOCK_SIZE);
    DevReadBlock(pBlock,readBlock);//physical Block 읽기
    for(int i=0; i<length; i++){
        pBuffer[i] = readBlock[i];
    }

    file->fileOffset += length;

    return length;
    //TODO: 실패시 -1 리턴.. 실패 조건?
}

int		CloseFile(int fileDesc)
{
    if(pFileDesc[fileDesc].bUsed == FALSE) return FAILED;
    //이미 없는 파일 -> FAILED
    pFileDesc[fileDesc].bUsed=FALSE;
    free(pFileDesc[fileDesc].pOpenFile);
    pFileDesc[fileDesc].pOpenFile = NULL;
    return SUCCESS;
}

int		RemoveFile(const char* szFileName)
{
    int pInodeNum = pathFinder(szFileName);     //부모 iNode
    int nInodeNum = pathFinder_n(szFileName);   //자신의 iNode

    /*제거할 파일 이름이 없을 경우*/
    if(pInodeNum == -1) return FAILED;
    if(nInodeNum == -1) return FAILED;

    /*제거될 파일이 Open되어 있을 경우*/
    for(int i=0; i<MAX_FD_ENTRY_MAX; i++){
        if(pFileDesc[i].pOpenFile->inodeNum==nInodeNum){
            return FAILED;
        }
    }
    /*********create file의 반대과정**********/
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(pInodeNum,pInode);
    DevReadBlock(pInode->dirBlockPtr[0],pBuf);
    DirEntry* dirEntry = (DirEntry *)pBuf;//상위 디렉토리를 불러온다.

    char * name = NameFinder(szFileName);
    int td = FindDirTable(dirEntry,name);//자식 테이블번호 불러오기

    /* 1-1 부모디렉토리 블럭정보 변경 */
    strcpy(dirEntry[td].name,"");//TODO: 빈 문자열.. 테케때 확인
    dirEntry[td].inodeNum = NONE;

    /* 1-2 부모디렉토리 Inode정보 변경 */
    pInode->allocBlocks--;
    pInode->size=(pInode->allocBlocks)*BLOCK_SIZE;
    DevWriteBlock(pInode->dirBlockPtr[0],pBuf);//수정정보 디스크에 저장
    free(pBuf);//쓰고나서 임시 Buf는 삭제...

    //TODO: szName Block이 없다고 확신할 수 있나?

    /* szName Inode 없애기 */
    GetInode(nInodeNum,pInode);//자기자신의 iNode 불러오기
    memset(pInode,0,sizeof(Inode));//0으로 다시 셋팅
    SetInode(nInodeNum,pInode);
    free(pInode);

    //bytemap 업데이트
    ResetInodeBytemap(nInodeNum);
    //FileSysInfo 변경
    pFileSysInfo->numAllocInodes--;
    DevWriteBlock(FILESYS_INFO_BLOCK,(char*)pFileSysInfo);
    
    return SUCCESS;
}

int		MakeDir(const char* szDirName)
{
    int pBlockNum = pathFinder(szDirName);//부모디렉토리 parsing
    if(pBlockNum == -1) return FAILED;

    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    DevReadBlock(pBlockNum,pBuf);
    DirEntry* dirEntry = (DirEntry *)pBuf;//상위 디렉토리를 불러온다.
    GetInode(dirEntry[0].inodeNum,pInode);

    // 생성할 디렉토리 블럭과 Inode
    int bn = GetFreeBlockNum();// GetFreeBlockNum : 8
    int in = GetFreeInodeNum();// FreeInode 1

    int nd = WhereNewDirOrder(dirEntry,szDirName);//상위 폴더에 빈칸을 찾아 할당한다.
    if(nd==FAILED) {
        free(pBuf);
        free(pInode);
        return FAILED;//동일한 이름이 있다면 FAILED
    }
    
    //부모디렉토리 블럭정보 변경
    strcpy(szDirName,dirEntry[nd].name);
    dirEntry[nd].inodeNum = in;
    //부모디렉토리 Inode정보 변경
    pInode->allocBlocks++;
    pInode->size=(pInode->allocBlocks)*BLOCK_SIZE;
    DevWriteBlock(pBlockNum,pBuf);//수정정보 디스크에 저장
    free(pBuf);//쓰고나서 임시 Buf는 삭제...

    /* 2 szName Directory 내부 폴더 초기화*/
    pBuf = (char *)malloc(BLOCK_SIZE);//(1) Block크기의 메모리 할당.
    dirEntry = (DirEntry *)pBuf;//(2) DirEntry구조체의 배열로 변환
    strcpy(".",dirEntry[0].name);//(3) DirEntry[1]의 변수들을 설정함.
    dirEntry[0].inodeNum = in;   //자기 자신의 iNode
    strcpy("..",dirEntry[1].name);
    dirEntry[1].inodeNum = 0;   // 상위 디렉토리의 iNode
    DevWriteBlock(bn,pBuf);//(4) 해당 directory파일을 디렉토리에 저장한다.
    free(pBuf);

    /* 2-2 szName Directory iNode 초기화*/
    GetInode(in,pInode);//자기자신의 iNode 불러오기
    pInode->allocBlocks = 2; // 자기자신에게 할당된 블록?? . & .. 일케 두개 아닌가..
    pInode->size = (pInode->allocBlocks)*(BLOCK_SIZE);
    pInode->type = FILE_TYPE_DIR;//왜 이게 int형이지? FileType형이 아닌가?
    pInode->dirBlockPtr[0] = bn;//0번은 "."  자기자신 bn(8)
    pInode->dirBlockPtr[1] = pBlockNum;//1번은 ".." 상위폴더 Parent Block Num
    SetInode(in,pInode);
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

int		RemoveDir(const char* szDirName)
{
    if(strcmp("/",szDirName)==0){
        //root 예외 처리해주고싶은데 지울수있으려나?
        //하위폴더가 항상 존재하나? 
    }
    /*********Make Dir의 반대과정**********/
    int pInodeNum = pathFinder(szDirName);     //부모 iNode
    int nInodeNum = pathFinder_n(szDirName);   //자신의 iNode

    /*제거할 디렉토리가 없을 경우*/
    if(pInodeNum == -1) return FAILED;
    if(nInodeNum == -1) return FAILED;

    /*지우려는 폴더의 하위 디렉토리가 존재할 경우*/
    Inode * nInode = (Inode *)malloc(sizeof(Inode));
    GetInode(nInodeNum,nInode);
    if(nInode->allocBlocks > 2){
        //".", ".." 이외에 다른 폴더가 있다면
        free(nInode);
        return FAILED;
    }

    
    /*Make의 반대과정*/
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(pInodeNum,pInode);
    DevReadBlock(pInode->dirBlockPtr[0],pBuf);
    DirEntry* dirEntry = (DirEntry *)pBuf;//상위 디렉토리를 불러온다.

    char * name = NameFinder(szDirName);
    int td = FindDirTable(dirEntry,name);//자식 테이블번호 불러오기

    /* 1-1 부모디렉토리 블럭정보 변경 */
    strcpy(dirEntry[td].name,"");//TODO: 빈 문자열.. 테케때 확인
    dirEntry[td].inodeNum = NONE;
    
    /* 1-2 부모디렉토리 Inode정보 변경 */
    pInode->allocBlocks--;
    pInode->size=(pInode->allocBlocks)*BLOCK_SIZE;
    DevWriteBlock(pInode->dirBlockPtr[0],pBuf);//수정정보 디스크에 저장
    free(pBuf);//쓰고나서 임시 Buf는 삭제...

    /* Block 없애기 */
    DevReadBlock(nInode->dirBlockPtr[0],pBuf);
    memeset(pBuf,0,sizeof(BLOCK_SIZE));
    DevWriteBlock(nInode->dirBlockPtr[0],pBuf);
    ResetBlockBytemap(nInode->dirBlockPtr[0]);//bytesMap
    free(pBuf);
    
    /* Inode 없애기 */
    GetInode(nInodeNum,pInode);//자기자신의 iNode 불러오기
    memset(pInode,0,sizeof(Inode));//0으로 다시 셋팅
    SetInode(nInodeNum,pInode);
    ResetInodeBytemap(nInodeNum);
    free(pInode);
    
    
    //FileSysInfo 변경
    pFileSysInfo->numAllocInodes--;
    pFileSysInfo->numAllocBlocks--;
    pFileSysInfo->numFreeBlocks++;
    DevWriteBlock(FILESYS_INFO_BLOCK,(char*)pFileSysInfo);
    
    return SUCCESS;
}

int   EnumerateDirStatus(const char* szDirName, DirEntryInfo* pDirEntry, int dirEntrys)
{
    /*해당 디렉토리를 구성하고 있는 디렉토리 엔트리들의 묶음을 리턴*/

    //DirectoryInfo a[40]; 이런게 2번째로 들어올 수 있음. 최대 크기를 dirEntry로
    //심지어 파일인데 DirectoryInfo가능한가? ㅇㅇ 이건 
    //openDir, ReadDir 이런거를 enum으로 선언하겠다는 뜻임.???

    /*디렉토리 이름이 없을 경우*/
    int nInodeNum = pathFinder_n(szDirName);
    if(nInodeNum == -1) return FAILED;

    char * pBuf = (char*)malloc(BLOCK_SIZE);
    Inode * nInode = (Inode *)malloc(sizeof(Inode));
    GetInode(nInodeNum,nInode);
    DevReadBlock(nInode->dirBlockPtr[0],pBuf);//Block 가져오기

    /*디렉토리가 아닐 경우*/
    if(nInode->type != FILE_TYPE_DIR) {
        free(pBuf);
        free(nInode);
        return FAILED;
    }
    //TODO: 근데 directory당  5개만 들어갈 수 있었던거 아닌가?
    for(int i=0; i<dirEntrys; i++){
        DirEntryInfo a;
        *(pDirEntry+i) = a;
        //return;
    }
    
    /*
    Parameters
        szDirName[in]: 디렉토리 이름 (절대 경로 사용).
        pDirEntry[out]: 디렉토리 엔터리들을 저장할 메모리 주소
        dirEntries[in]: 읽을 디렉토리 엔터리의 개수

        디렉토리 엔터리 개수가 40이지만, dirEntries가 60으로 입력했을 때,
        리턴되는 값은 유효한 디렉토리 엔터리 개수인 40을 리턴해야 한다. 

        전체 디렉토리 엔터리 개수가 40이지만 dirEntry가 20으로 입력되었을 때,
        리틴되는 값은 20이며, 20개의 디렉토리 엔터리의 내용이 pDirEntry로 리턴되어야 한다
    */
    //tmp - 1 a -b 2 b 3 c
    //3개파일에 대한 정보를 a배열에 채워서 리턴하게 됨.
    //반환값은 tmp 디렉토리에 들어있는 갯수만큼을 리턴하면 된다.
    // 3이라는 개수만큼 포문을 돌릴 것임. 
}

void	CreateFileSystem()
{
    /*가상디스크 생성 format동작*/
    DevCreateDisK();
    OpenFileSystem();
    //TODO: 여기서 호출하는건가 메인에서 호출하는건가...이건 테케가 나와봐야 알듯.
    FileSysInit();//(0)디스크 초기화

    //(1) Block bytemap의 byte 7부터 검색하여 free block검색 7
    //(2) Inode bytemap의 byte 0부터 검색하여 free inode검색 0
    int bn = GetFreeBlockNum();
    int in = GetFreeInodeNum();

    //루트디렉토리 생성
    //(3)Block크기 메모리 할당 -> root디렉토리에 대한 메모리
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    DirEntry* rootDirEntry = (DirEntry *)pBuf;
    //(4) DirEntry구조체 배열로 변환 DirEntry[0]의 주소를가리킴
    //(5) DirEntry[0]의 변수들을 설정함
    strcpy(".",rootDirEntry[0].name);//루트 디렉토리 자기 자신.
    rootDirEntry[0].inodeNum = in; //빈 inode 할당 : 0

    DevWriteBlock(bn,pBuf);// (6) Block 7번에 루트Dir추가
    free(pBuf);

    pBuf = (char *)malloc(BLOCK_SIZE);//(7) Block크기 메모리 할당 

    FileSysInfo * fileSysInfo = (FileSysInfo *)pBuf;
    //(8) FileSysInfo로 형변환 한다:FilesysInfo[0]의 주소를 가리킨다.
    fileSysInfo->blocks = 512;
    fileSysInfo->rootInodeNum = 0;
    fileSysInfo->diskCapacity = FS_DISK_CAPACITY;
    fileSysInfo->numAllocBlocks;
    fileSysInfo->numFreeBlocks;
    fileSysInfo->numAllocInodes;
    fileSysInfo->inodeBytemapBlock = INODE_BYTEMAP_BLOCK_NUM;//1
    fileSysInfo->blockBytemapBlock = BLOCK_BYTEMAP_BLOCK_NUM;//2
    fileSysInfo->inodeListBlock = INODELIST_BLOCK_FIRST;//3
    fileSysInfo->dataRegionBlock = 7;

    pFileSysInfo = fileSysInfo;
    //먼저 초기 디스크 파라미터에 맞게 초기화 시킨 후
    //설정된 값에 맞게 값을 변경시키고 Disk에 저장한다.
    pFileSysInfo->numAllocBlocks++;
    pFileSysInfo->numAllocInodes++;
    pFileSysInfo->numFreeBlocks--;

    //(9) 해당 블록을 DevWriteBlock을 사용해 Block 0에 저장한다.
    DevWriteBlock(0,pBuf);
    //rebooting때에도 변경된 상태로 유지 -> ByteMap상태를 변경
    SetBlockBytemap(bn);
    SetInodeBytemap(in);

    //아까 루트 Inode를 할당했다고 했는데,,, 난 왜 지금 할당하지...
    Inode * rootInode = (Inode *)malloc(sizeof(Inode));//rootInode 할당
    GetInode(0,rootInode);//rootDirInode획득한다. 근데 아직 저장된거없던데..

    rootInode->allocBlocks = 1;//root디렉토리 파일에 한개block 할당
    rootInode->type = FILE_TYPE_DIR;
    rootInode->size = (rootInode->allocBlocks)*(BLOCK_SIZE);
    //디렉토리를 구성하고 있는 블럭 개수 * 512 bytes = 디렉토리의 크기
    rootInode->dirBlockPtr[0] = bn;//7번블록
    SetInode(0,rootInode);
 
}

void	OpenFileSystem()
{
    DevOpenDisk();
    //mount 작업
    //가상디스크라는 파일을 open하는 동작만 수행한다.
}

void	CloseFileSystem()
{
    DevCloseDisk();
    //가상디스크 파일을 close한다.
}

int		GetFileStatus(const char* szPathName, FileStatus* pStatus){
    /*stat 함수와 동일한 동작을 한다.*/
    int nInodeNum = pathFinder_n(szPathName);//해당 파일의 inode획득
    if(nInodeNum == -1) return FAILED;//해당 path를 찾지 못했다면 failed

    //해당 path가 지정하는 inode를 찾아서
    Inode* pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(nInodeNum,pInode);

    //Status초기화
    pStatus->allocBlocks = pInode->allocBlocks;
    for(int i=0; i<NUM_OF_DIRECT_BLOCK_PTR; i++){
        pStatus->dirBlockPtr[i] = pInode->dirBlockPtr[i];
    }
    pStatus->size = pInode->size;
    pStatus->type = pInode->type;
    return SUCCESS;
}
