

#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "disk.h"
#include "fs.h"

FileSysInfo* pFileSysInfo;
typedef struct _TableIndex{
    int pi;//point index
    int ti;//table index
}TableIndex;
typedef struct _PathInfo{
    char name[MAX_NAME_LEN];//path이름
    int pInodeNum;//해당 path를 담는 parent의 inode
}PathInfo;

#define MODE (0644)
#define BLOCK_NUM (512)
#define NONE (-15)
#define DEBUGGING (0)
enum {FAILED=-1, SUCCESS};
enum {FALSE=0, TRUE=1};
enum {PARENT=0, CHILD};
int WhereIsInodeBlock(int indoeno);
int WhereIsInodeOrder(int indoeno);
TableIndex WhereNewDirOrder(int in);
TableIndex FindDirTable(int in, const char * targetName);
int AllocNewBlock(int inode);
PathInfo PathFinder(const char * szFileName);
int UsingInode(int in);
int UsingBlock(int bn);
int IsEmptyDir(int in);
void print_pSysInfo();
void printDirEntry(int in);
void DirEntryInit(DirEntry * direntry);
void BlockInit(char * pBuf);
void Test(int in);
int FindDescriptorTable();
