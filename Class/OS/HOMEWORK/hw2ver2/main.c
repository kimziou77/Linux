#include "Headers.h"
#include "TestCase1.h"
#include "TestCase2.h"
#include "TestCase3.h"
#include "TestCase4.h"


int main(int argc, char* argv[]){
	
	
	int TcNum;
	thread_t tid1, tid2, tid3, tid4, tid5;

	if(argc!=2){
		perror("Input TestCase Number!");
		exit(0);
	}

	Init();
	printf("Start! My Pid is : %d\n",mainPid);
	TcNum=atoi(argv[1]);
	int pid;
	switch(TcNum)
	{
		case 1:
			pid = thread_create(&tid1,NULL,0,(void*)TestCase1,0);
			break;

		case 2:
			thread_create(&tid2,NULL,0,(void*)TestCase2,0);
			break;
		case 3:
			thread_create(&tid3,NULL,0,(void*)TestCase3,0);
			break;
		case 4:
			thread_create(&tid4,NULL,0,(void*)TestCase4,0);
			break;
	}
	
	RunScheduler();

	while(1){}
	return 0;
}
