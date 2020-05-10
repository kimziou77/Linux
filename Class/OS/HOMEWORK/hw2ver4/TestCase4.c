#include "TestCase4.h"

thread_t tidArray[TOTAL_THREAD_NUM];

void* Tc4ThreadProc(void* param)
{
	thread_t tid = 0;

	int* retVal;
	int currentNum=(*((int*)param));
	tid = thread_self();

	int i;
	for(i=0;i<5;i++){
		printf("Tc4ThreadProc: my thread id (%d), arg is (%d)\n", (int)tid, currentNum);
		if(i==1&&currentNum!=1){
			thread_resume(tidArray[currentNum/2]);
		}
		sleep(2);
	}
	retVal = (int*)param;
	thread_exit(retVal);

	return NULL;
}


void TestCase4(void)
{

	int arr[TOTAL_THREAD_NUM]={0,1,2,3,4,5};

	for(int i=0;i<TOTAL_THREAD_NUM;i++){
		thread_create(&tidArray[i], NULL, (i/2)+1, (void*)Tc4ThreadProc,(void*) &(arr[i]));	
		thread_suspend(tidArray[i]);
	}

	thread_resume(tidArray[5]);

	int* retVal;
	thread_join(tidArray[1],(void **)&retVal);
	printf("Thread [ %d ] is finish Return : [ %d ] \n",(int)tidArray[1], *retVal);

	assert(0);

	return ;
}
