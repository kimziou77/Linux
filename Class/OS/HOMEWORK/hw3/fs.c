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


int		MakeDir(const char* szDirName)
{
/*
- 디렉토리를 생성한다.
- Parameters
 szDirName[in]: 생성할 디렉토리 이름 (절대 경로 사용).
- Return
성공하면, 0을 리턴한다. 실패했을때는 -1을 리턴한다. 실패원인은 생성하고자 하는 디
렉토리의 이름과 동일한 디렉토리 또는 파일이 존재할 경우.
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
    DevCreateDisK();//가상 디스크 생성함수.
      
}


void	OpenFileSystem()
{

}


void	CloseFileSystem()
{

}
