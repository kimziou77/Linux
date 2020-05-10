#include "TestCase2.h"
#include "Headers.h"

void* Tc2ThreadProc(void* param)
{
	
	thread_t tid = 0;
	tid = thread_self();
	while (1)
	{
		printf("Tc2ThreadProc: my thread id:(%d), arg : %d | pid : %d\n", (int)tid, *((int*)param),getpid());
		sleep(2);
	}
}

void TestCase2(void)
{

	thread_t tid[TOTAL_THREAD_NUM];
	int i = 0;
	int j = 0;
	int i1 = 1, i2 = 2, i3 = 3;

	thread_create(&tid[0], NULL, 0, (void*)Tc2ThreadProc, (void*)&i1);	
	thread_create(&tid[1], NULL, 0, (void*)Tc2ThreadProc, (void*)&i2);	
	thread_create(&tid[2], NULL, 0, (void*)Tc2ThreadProc, (void*)&i3);	

	printf("<suspend start>\n");
	for (i = 0; i < TOTAL_THREAD_NUM;i++)
	{	
		if(DEBUGGING) printf("[=]sleep2before\n");
		sleep(2);
		if(DEBUGGING) printf("[=]sleep2after\n");
		// print_all();

		if (thread_suspend(tid[i]) == -1)
		{
			printf("TestCase2: Thread suspending Failed..\n");
			assert(0);
		}
		if(DEBUGGING) print_all();
		printf("suspend succeess\n\n");
		Thread *temp = pWaitingQueueHead;

		printf("current waiting queue : ");
		for(;temp!=NULL;temp=temp->phNext){
			printf(" %d",temp->pid);
			if(temp->status != 2)
			{
				printf("TestCase2: Thread is not suspended\n");
				assert(0);
			}

		}
		printf("\n");
	}


	printf("\n");
	printf("<resume start>\n");
	for (i=0;i<TOTAL_THREAD_NUM;i++)
	{
		if(DEBUGGING) printf("[=]resume before\n");
		sleep(2);
		if(DEBUGGING) printf("[=]resume after\n");
		printf("tid : %d resume \n",tid[i]);
		if(thread_resume(tid[i])==-1)
		{
			printf("Testcase2: Thread resume Failed\n");
			assert(0);
		}

		Thread *temp = pReadyQueueEnt[0].pHead;

		printf("current ready[0] queue : ");
		for(;temp!=NULL;temp=temp->phNext){
			printf(" %d",temp->pid);
			if(temp->status != 1)
			{
				printf("TestCase2: Thread is not resumed\n");
				assert(0);
			}

		}
		printf("\n");

	}
	sleep(2);

	printf("\n");
	printf("<suspend start>\n");
	for (i = TOTAL_THREAD_NUM-1 ; i >=0 ; i--)
	{	
		sleep(2);

		if (thread_suspend(tid[i]) == -1)
		{
			printf("TestCase2: Thread suspending Failed..\n");
			assert(0);
		}

		Thread *temp = pWaitingQueueHead;

		printf("current waiting queue : ");
		for(;temp!=NULL;temp=temp->phNext){
			printf(" %d",temp->pid);
			if(temp->status != 2)
			{
				printf("TestCase2: Thread is not suspended\n");
				assert(0);
			}
		}
		printf("\n");

	}

	printf("\n");
	printf("<resume start>\n");
	for (i = TOTAL_THREAD_NUM-1 ; i >=0 ; i--)
	{
		sleep(2);

		if(thread_resume(tid[i])==-1)
		{
			printf("Testcase2: Thread resume Failed\n");
			assert(0);
		}

		Thread *temp = pReadyQueueEnt[0].pHead;

		printf("current ready[0] queue : ");
		for(;temp!=NULL;temp=temp->phNext){
			printf(" %d",temp->pid);
			if(temp->status != 1)
			{
				printf("TestCase2: Thread is not resumed\n");
				assert(0);
			}

		}
		printf("\n");

	}
	sleep(2);

	printf("\n");
	printf("<cancel start>\n");
	for (i=0;i<TOTAL_THREAD_NUM;i++)
	{
		sleep(2);
		if(thread_cancel(tid[i])==-1)
		{
			printf("Testcase2: Thread cancel Failed\n");
			assert(0);
		}

		Thread *temp = pReadyQueueEnt[0].pHead;

		printf("current ready[0] queue : ");
		for(;temp!=NULL;temp=temp->phNext){
			printf(" %d",temp->pid);
		}
		printf("\n");

	}
	printf("\n");
	printf("Every thread is cancelled,\nTestcase2 is end, Please Ctrl+C\n");
	print_all();

	return ;
}
