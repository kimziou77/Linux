#include "fs.h"
#include "disk.h"
#include <stdio.h>

int main(){
	printf("Hello OS World\n");

	int blknum, inodenum;
	Inode inode, *pInode;

	FileSysInit();
	inodenum = GetFreeInodeNum();
	SetInodeBytemap(inodenum);

	blknum = GetFreeBlockNum();
	SetBlockBytemap(blknum);

	inode.size = 0;
	inode.allocBlocks = 0;
	PutInode(inodenum, &inode);

	pInode = malloc(sizeof(Inode));
	GetInode(inodenum, pInode);

	return 0;
}
