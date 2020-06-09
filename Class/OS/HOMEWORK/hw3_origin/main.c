#include "Headers.h"

int main(){
	if(DEBUGGING) printf("[+] Main \n");
	CreateFileSystem();//가상디스크 생성및 초기화

	// system("clear");
	char * tmp = (char *)malloc(BLOCK_SIZE);
	// for(int i=0; i< 7 ; i++){
	// 	DevReadBlock(i,tmp);
	// 	for(int j=0; j<512 ;j++){
	// 		printf("%d ",tmp[j]);
	// 	}
	// 	printf("\n----------------------\n\n");
	// }
	
	char dirName[MAX_NAME_LEN];
	
	MakeDir("/tmp");
	// char * fileInfo ;
		for (int i = 0; i < 34; i++)
	{
		memset(dirName, 0, MAX_NAME_LEN);
		sprintf(dirName, "/tmp/dev%d", i);
		MakeDir(dirName);
	}
	print_pSysInfo();
	
	FileStatus status;
	if(GetFileStatus("/tmp/tmp32",&status) < 0){
		printf("File not found.\n");
		return 0;
	}
	printf("fileSize: %d , file Blocks: %d, 1st block ptr: %d\n",
			status.size, status.allocBlocks, status.dirBlockPtr[0]) ;

	CloseFileSystem();
	
	if(DEBUGGING) printf("[-] Main \n");
	return 0;
}

void test1(){
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
}

void test2(){
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
	
}
void test3(){
	// FileStatus status;
	// if(GetFileStatus("/tmp/a.c",&status) < 0){
	// 	printf("File not found.\n");
	// 	return 0;
	// 	// exit(0);
	// }
	// printf("fileSize: %d , file Blocks: %d, 1st block ptr: %d\n",
	// 		status.size, status.allocBlocks, status.dirBlockPtr[0]) ;
	//  int i;
	//  int count;
	 
	// DirEntryInfo pDirEntryInfo[20];
	// if ((count = EnumeratreDirStatus("/usr/home/kim", pDirEntryInfo, 20)) <	0){
	// 	exit(0); // program terminated due to the error return of the function.
	// }
	// for (i = 0;i < count;i++){
	// 		printf("directory entry:%s, type:%d, inode number:%d\n ",
	// 		pDirEntryInfo[i].name, pDirEntryInfo[i].type,	pDirEntryInfo[i].inodeNum);
	// }

}
