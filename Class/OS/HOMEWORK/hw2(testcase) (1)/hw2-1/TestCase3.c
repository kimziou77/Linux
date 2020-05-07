#include "TestCase3.h"

void* Tc3ThreadProc(void* param)
{
	thread_t tid = 0;
	int count = 0;
	int i;
	int* retVal;
	tid = thread_self();
	for(int i=0;i<5;i++){
		sleep(2);
		printf("Tc3ThreadProc: my thread id (%d), arg is (%d)\n", (int)tid, *((int*)param));
		count++;
	}
	retVal = (int*)param;
	thread_exit(retVal);
	return NULL;
}


void TestCase3(void)
{
	thread_t tid[TOTAL_THREAD_NUM];
	int result[TOTAL_THREAD_NUM];

	int arr[TOTAL_THREAD_NUM]={0,1,2,3,4,5,6,7,8};

	for(int i=0;i<TOTAL_THREAD_NUM;i++){

		thread_create(&tid[i], NULL, (i/3)+1, (void*)Tc3ThreadProc,(void*) &(arr[i]));	
	}

	for(int i=0;i<TOTAL_THREAD_NUM;i++)
	{
		int* retVal;
		thread_join(tid[i],(void **)&retVal);

		printf("Thread [ %d ] is finish Return : [ %d ] \n",(int)tid[i], *retVal);
	}

	return ;
}

