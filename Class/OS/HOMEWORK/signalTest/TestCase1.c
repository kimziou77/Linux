#include "TestCase1.h"

void* Tc1ThreadProc(void* param)
{
	
	thread_t tid = 0;
	int count = 0;
	int i;
	int* retVal;

	tid = thread_self();

	for(int i=0;i<5;i++){
		printf("%d sleep\n",getpid());
		sleep(2);
		printf("%d wakeup\n",getpid());
		printf("Tc1ThreadProc: my thread id (%d), arg is (%d) %d pid is : %d \n", (int)tid, *((int*)param),i,getpid());
		count++;
		//TODO: 이 카운트는 뭐야 
	}
	retVal = (int*)param;
	thread_exit(retVal);
	return NULL;
}

void TestCase1(void)
{
	thread_t tid[TOTAL_THREAD_NUM];
	int result[TOTAL_THREAD_NUM];

	int i = 0, i1 = 1, i2 = 2, i3 = 3, i4 = 4, i5 = 5;

	thread_create(&tid[0], NULL, 1, (void*)Tc1ThreadProc,(void*) &i1);	
	thread_create(&tid[1], NULL, 1, (void*)Tc1ThreadProc,(void*) &i2);	
	thread_create(&tid[2], NULL, 1, (void*)Tc1ThreadProc,(void*) &i3);	
	thread_create(&tid[3], NULL, 1, (void*)Tc1ThreadProc,(void*) &i4);	
	thread_create(&tid[4], NULL, 1, (void*)Tc1ThreadProc,(void*) &i5);

	for(i=0;i<TOTAL_THREAD_NUM;i++)
	{
		int* retVal;	

		thread_join(tid[i],(void **)&retVal);
		printf("Thread [ %d ] is finish Return : [ %d ] \n",(int)tid[i], *retVal);
	}

	return ;
}
