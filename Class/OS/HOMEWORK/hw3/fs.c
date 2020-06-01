#include "Headers.h"

int		CreateFile(const char* szFileName)
{
    //파일이 존재하지 않으면 생성하고 open함 -> O_RDWR | O_CREAT -> fd반환
    //이미 파일이 존재하면 실패 해야하고 -1 리턴해야됨 -> O_EXCL
    //szFileName : open할 파일이름 -> 파일이름은 절대경로임
    int fd = open(szFileName, O_RDWR|O_CREAT|O_EXCL, MODE);

    return fd;
/*
    성공하면, file descriptor를 리턴.
    이 file descriptor는 file descriptor table의entry의 index값으로 정의된다.
    실패했을때는 -1을 리턴한다
*/
}

int		OpenFile(const char* szFileName)
{
    //파일이 이미 존재하면 해당 open하고, file descriptor를 반환함./
    //szFileName : open할 파일 이름 , 파일 이름은 절대경로임.
    int fd = open(szFileName, O_RDWR);//TODO: fd가 전역으로 있는데,,, 흠
    return fd;
/*

- Return
    성공하면, file descriptor를 리턴한다. 이 file descriptor는 file descriptor table의 entry의 index값으로 정의된다. 실패했을때는 -1을 리턴한다.
*/
}


int		WriteFile(int fileDesc, char* pBuffer, int length)
{

/*
- open된 파일에 데이터를 저장한다.
- Parameters
 fileDesc[in]: file descriptor.
 pBuffer[in]: 저장할 데이터를 포함하는 메모리의 주소
 length[in]: 저장될 데이터의 길이
- Return
성공하면, 저장된 데이터의 길이 값을 리턴한다. 실패했을때는 -1을 리턴한다. 
*/
}

int		ReadFile(int fileDesc, char* pBuffer, int length)
{
/*
- open된 파일에서 데이터를 읽는다.
- Parameters
 fileDesc[in]: file descriptor.
 pBuffer[out]: 읽을 데이터를 저장할 메모리의 주소
 length[in]: 읽을 데이터의 길이
- Return
성공하면, 읽은 데이터의 길이 값을 리턴한다. 실패했을때는 -1을 리턴한다. 
*/
}


int		CloseFile(int fileDesc)
{
    

/*
- open된 파일을 닫는다.
- Parameters
 fileDesc[in]: file descriptor.
- Return
성공하면, 0을 리턴한다. 실패했을때는 -1을 리턴한다
*/
}

int		RemoveFile(const char* szFileName)
{
/*
- 파일을 제거한다. 단, open된 파일을 제거할 수 없다.
- Parameters
 szFileName[in]: 제거할 파일 이름. 단, 파일 이름은 절대 경로임.
- Return
성공하면, 0를 리턴한다. 실패했을때는 -1을 리턴한다. 실패 원인으로 (1) 제거할 파
일 이름이 없을 경우, (2) 제거될 파일이 open되어 있을 경우.
*/
}


int		MakeDir(const char* szDirName)// 항상 루트부터 들어오는건가?
{
    
    int bn = GetFreeBlockNum();//(1)  GetFreeBlockNum -> Block 8 szDirName을 위한 block
    int in = GetFreeInodeNum();//                        szDirName의 inode  1
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(0,pInode);//0번..
    

    //Root Directory Block에 szName폴더를 생성한다. 상위폴더없고, 하위폴더로 szName.
    char * pBuf = (char*)malloc(BLOCK_SIZE);
    int rBlockNum = pInode->dirBlockPtr[0];//root Inode의 directBlockPtr을 읽는다. -> 7
    DevReadBlock(rBlockNum,pBuf);//(2) Block 7을 디스크에서 읽는다. -> root디렉토리 블록
    DirEntry* dirEntry = (DirEntry *)pBuf;//pBuf를 dirEntry로 받아서 수정한 후 pBuf를 넣으면 수정이 되나?
    strcpy(szDirName,dirEntry[1].name);//(3) DirEntry[1]의 변수들을 설정함.
    dirEntry[1].inodeNum = in;
    DevWriteBlock(rBlockNum,pBuf);//(4) Block 7을 디스크에 저장한다.

    //디렉토리에서 빈 엔트리를 찾아가는 방법을 특정 문자를 넣어서 구분하도록 하자. ->null이라던지 문자열이라던지를 넣고 비어있는지 아닌지를 확인하도록 하자.

    //szName에 대한 directory를 만든다.
    pBuf = (char *)malloc(BLOCK_SIZE);//(1) Block크기의 메모리 할당.
    dirEntry = (DirEntry *)pBuf;//(2) DirEntry구조체의 배열로 변환

    strcpy(".",dirEntry[0].name);//(3) DirEntry[1]의 변수들을 설정함.
    dirEntry[0].inodeNum = 1;   //자기 자신

    strcpy("..",dirEntry[1].name);
    dirEntry[1].inodeNum = 0;   //상위 디렉토리(root(0))
    DevWriteBlock(bn,pBuf);//(4) Block 7을 디스크에 저장한다.

    //szName 파일의 iNode를 변경하고 디스크에 저장한다.

    GetInode(in,pInode);//아까 Get한걸 Set하지는 않았는데,,, 문제가 되지 않으려나 in: 1
    pInode->allocBlocks;
    pInode->size;
    pInode->type = FILE_TYPE_DIR;
    pInode->dirBlockPtr[0] = bn;//bn : 8
    SetInode(in,pInode);

    //bytemap들을 업데이트 해주어야 됨.
    SetBlockBytemap(bn);
    SetInodeBytemap(in);

    //FileSysInfo를 변경해주어야 한다.
    DevWriteBlock(FILESYS_INFO_BLOCK,pBuf);//FileSysInfo를 디스크에서 읽고 변경한다..
    FileSysInfo * filesysinfo;//이렇게는 아니지만 이런느낌으로
    filesysinfo->numAllocBlocks++;
    filesysinfo->numFreeBlocks--;
    filesysinfo->numAllocInodes++;
    DevWriteBlock(0,pBuf);

    /*
    성공하면, 0을 리턴한다. 실패했을때는 -1을 리턴한다.
    실패원인은 생성하고자 하는 디렉토리의 이름과 동일한 디렉토리 또는 파일이 존재할 경우.
    mkdir 시스템콜 https://www.it-note.kr/205?category=1068195
    */
}


int		RemoveDir(const char* szDirName)
{
/*
- 디렉토리를 제거한다. 단, 리눅스 파일 시스템처럼 빈 디렉토리만 제거가 가능하다.
- Parameters
 szDirName[in]: 제거할 디렉토리 이름 (절대 경로 사용).
- Return
성공하면, 0을 리턴한다. 실패했을때는 -1을 리턴한다. 실패 원인으로, (1) 디렉토리에
파일 또는 하위 디렉토리가 존할 경우, (2) 제거하고자 하는 디렉토리가 없을 경우.
*/
}

int   EnumerateDirStatus(const char* szDirName, DirEntryInfo* pDirEntry, int dirEntrys)
{
/*
 설명이 매우 긺...
*/
}


void	CreateFileSystem()
{
    //가상디스크 생성
    DevCreateDisK();
    FileSysInit();//(0) 디스크 초기화 Block 0 ~ 511까지 0으로 초기화
    
    int bn = GetFreeBlockNum();//(1) Block bytemap의 byte 7부터 검색하여 free block검색,
    int in = GetFreeInodeNum();//(2) Inode bytemap의 byte 0부터 검색하여 free inode검색.

    //루트디렉토리 생성
    char * pBuf = (char*)malloc(BLOCK_SIZE);//(3)Block크기 메모리 할당 -> root디렉토리에 대한 메모리
    DirEntry* dirEntry = (DirEntry *)pBuf;//이렇게 해도 되는거겠지...?ㅜㅜ 수정 되겠지..?ㅜㅠㅜ
    //(4) DirEntry구조체 배열로 변환 -> 접근할 수 있는 메모리 크기가 dirEntry씩 증가하고 감소함...
    //(5) DirEntry[0]의 변수들을 설정함
    strcpy(".",dirEntry[0].name);
    dirEntry[0].inodeNum = in;
    /*
    근데 루트 INode는 아직 할당 안해줬는데..????? Inode num을 0이라고 할 수 있나??
     루트  INode의 디렉토리 num : 7
     direct point가 7을 가리키도록 해야 한다.
    */
    
    DevWriteBlock(bn,pBuf);// (6) 변경된 블록을 DevWriteBlock을 사용해 Block 7번에 저장

    //TODO: 그러면 저기 fs.h 에 있는 pFileSysInfo는 뭐지..?
    pBuf = (char *)malloc(BLOCK_SIZE);//(7) Block크기 메모리 할당 
    FileSysInfo * fileSysInfo = (FileSysInfo * )pBuf;//(8) FileSysInfo로 type변환함.
    //파일의 상태에 따라서 각 변수를 초기화 시켜줘야 함...
    //0번으로 초기화 시켜줘야 하고...

    DevWriteBlock(0,pBuf);//(9) 해당 블록을 DevWriteBlock을 사용해 Block 0에 저장한다.
    //루트 디렉토리 파일에 한 개의 블록이 추가되었으니
    //해당 inode의 logical block 0 에 대응하는 direct block pointer[0]을 변경해야 한다.
    SetBlockBytemap(bn);
    SetInodeBytemap(in);

    // Inode->Disk 저장 로직 (루트 디렉토리)
    Inode * pInode = (Inode *)malloc(sizeof(Inode));
    GetInode(0,pInode);
    pInode->allocBlocks = 1;
    pInode->type = FILE_TYPE_DIR;
    pInode->size = (pInode->allocBlocks)*(BLOCK_SIZE);
    //디렉토리를 구성하고 있는 블럭 개수 * 512 bytes = 디렉토리의 크기
    pInode->dirBlockPtr[0] = bn;
    SetInode(0,pInode);


    //FileSysInfo초기화 하고 malloc으로 할당한 후에 포인터가 가르킈도록
    pFileSysInfo->blocks = 512;// 디스크에 저장된 전체 블록 개수
    pFileSysInfo->rootInodeNum = 0;// 루트 inode의 번호
    pFileSysInfo->diskCapacity = FS_DISK_CAPACITY;// 디스크 용량 (Byte 단위)
    pFileSysInfo->numAllocBlocks;// 파일 또는 디렉토리에 할당된 블록 개수
    pFileSysInfo->numFreeBlocks;//  할당되지 않은 블록 개수
    pFileSysInfo->numAllocInodes;//할당된 inode 개수 
    pFileSysInfo->blockBytemapBlock = BLOCK_BYTEMAP_BLOCK_NUM;
    pFileSysInfo->inodeBytemapBlock = INODE_BYTEMAP_BLOCK_NUM;
    pFileSysInfo->inodeListBlock = INODELIST_BLOCK_FIRST;
    pFileSysInfo->dataRegionBlock = INODELIST_BLOCK_FIRST + INODELIST_BLOCKS;
    
}


void	OpenFileSystem()
{

}


void	CloseFileSystem()
{

}

void FileSysInit(){
    /*
    (1) Block 크기의 메모리 할당하고 0으로 채운다
    (2) DevWriteBlock 함수를 통해 메모리를 Block 0부터 6까지 저장한다.
    */
   
    char * pBuf = (char*)malloc(BLOCK_SIZE);//Block크기 메모리 할당
    //FileSysInfo - Block 0
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
