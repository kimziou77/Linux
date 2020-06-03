#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include <string.h>
#include "Headers.h"
int main(){
	printf("Hello OS World\n");
	// printf("%d",sizeof("."));
	// mkdir("./tmp/tmp2",777);
	// const char * szDirName = "dir/dir1/dir2/dir3";
	// char parsing[]="";
	// strcmp(szDirName, parsing);
	// printf("%s\n",parsing);
	// char * tmp;
    // char * ptr = strtok(parsing,"/");
    // while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
    // {
	// 	printf("%s\n",ptr);
    //     ptr = strtok(NULL, "/");      // 다음 문자열을 잘라서 포인터를 반환
    // }

	FileStatus status;
	if(GetFileStatus("/tmp/a.c",&status) < 0){
		printf("File not found.\n");
		return 0;
		// exit(0);
	}
	printf("fileSize: %d , file Blocks: %d, 1st block ptr: %d\n",
			status.size, status.allocBlocks, status.dirBlockPtr[0]) ;

	// DevCreateDisk();
	// int blknum, inodenum;
	// Inode inode, *pInode;

	// FileSysInit();
	// inodenum = GetFreeInodeNum();
	// SetInodeBytemap(inodenum);

	// blknum = GetFreeBlockNum();
	// SetBlockBytemap(blknum);

	// inode.size = 0;
	// inode.allocBlocks = 0;
	// PutInode(inodenum, &inode);

	// pInode = malloc(sizeof(Inode));
	// GetInode(inodenum, pInode);
	// printf("Finish\n");
	// return 0;
}
