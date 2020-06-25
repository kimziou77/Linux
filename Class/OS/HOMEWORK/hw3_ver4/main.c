#include "Headers.h"

int main(){
	if(DEBUGGING) printf("[+] Main \n");
	// CreateFileSystem();
	OpenFileSystem();

	// char dirName[MAX_NAME_LEN];
	// MakeDir("/tmp");
	// MakeDir("/home");
	// /* make home directory */
	// for (int i = 0; i < 32; i++)
	// {
	// 	memset(dirName, 0, MAX_NAME_LEN);
	// 	sprintf(dirName, "/home/user%d", i);
	// 	MakeDir(dirName);
	// }
	printf("@\n");

	char * pBuf = (char *)malloc(BLOCK_SIZE);
	DevReadBlock(0,pBuf);
	FileSysInfo * filesysinfo = (FileSysInfo *)pBuf;
	printf("%d\n",filesysinfo->numAllocBlocks);
	
	
	if(DEBUGGING) printf("[-] Main \n");
	return 0;
}
