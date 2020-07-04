#include "TestCase4.h"

int flag[6]={0,};

void* Tc4ThreadPing(void* param)
{
	pmqd_t mq1,mq2,pmq;
	char msg[MAX_MSG_LEN];
	int i;
	int msg_prio;
	int* retVal;
	
	mq1=pmq_open("mq2",O_CREAT,O_RDWR,NULL);
	mq2=pmq_open("mq3",O_CREAT,O_RDWR,NULL);
	pmq=pmq_open("mq1",O_CREAT,O_RDWR,NULL);

	for(i=1;i<=5;i++){
		memset(msg,0,MAX_MSG_LEN);
		sprintf(msg,"%d",*((int*)param));
		pmq_send(mq1,msg,MAX_MSG_LEN,0);
		memset(msg,0,MAX_MSG_LEN);
		pmq_receive(mq2,msg,MAX_MSG_LEN,&msg_prio);
		flag[msg[0]-48]++;
	}

	memset(msg,0,MAX_MSG_LEN);
	sprintf(msg,"Ping complete");
	pmq_send(pmq,msg,MAX_MSG_LEN,0);

	pmq_close(mq2);
	pmq_close(mq1);
	pmq_close(pmq);

	retVal=(int*)param;
	thread_exit(retVal);
	return NULL;
}

void* Tc4ThreadPong(void* param)
{
	pmqd_t mq1,mq2,pmq;
	char msg[MAX_MSG_LEN];
	int i;
	int msg_prio;
	int* retVal;

	mq1=pmq_open("mq2",O_CREAT,O_RDWR,NULL);
	mq2=pmq_open("mq3",O_CREAT,O_RDWR,NULL);
	pmq=pmq_open("mq1",O_CREAT,O_RDWR,NULL);

	for(i=1;i<=5;i++){
		memset(msg,0,MAX_MSG_LEN);
		pmq_receive(mq1,msg,MAX_MSG_LEN,&msg_prio);
		flag[msg[0]-48]++;
		memset(msg,0,MAX_MSG_LEN);
		sprintf(msg,"%d",*((int*)param));
		pmq_send(mq2,msg,MAX_MSG_LEN,0);
	}

	memset(msg,0,MAX_MSG_LEN);
	sprintf(msg,"Pong complete");
	pmq_send(pmq,msg,MAX_MSG_LEN,0);

	pmq_close(mq2);
	pmq_close(mq1);
	pmq_close(pmq);

	retVal=(int*)param;
	thread_exit(retVal);	
	return NULL;
}

void TestCase4(void)
{
	thread_t tid[6];
	pmqd_t mq;
	char msg[MAX_MSG_LEN];
	int arr[6]={0,1,2,3,4,5};
	int i;

	mq=pmq_open("mq1",O_CREAT,O_RDWR,NULL);

	thread_create(&tid[0], NULL, 1, (void*)Tc4ThreadPong,(void*)&arr[0]);
	thread_create(&tid[1], NULL, 1, (void*)Tc4ThreadPong,(void*)&arr[1]);
	thread_create(&tid[2], NULL, 1, (void*)Tc4ThreadPong,(void*)&arr[2]);
	thread_create(&tid[3], NULL, 1, (void*)Tc4ThreadPing,(void*)&arr[3]);
	thread_create(&tid[4], NULL, 1, (void*)Tc4ThreadPing,(void*)&arr[4]);
	thread_create(&tid[5], NULL, 1, (void*)Tc4ThreadPing,(void*)&arr[5]);


	for(int j=0;j<6;j++){
		memset(msg,0,MAX_MSG_LEN);
		pmq_receive(mq,msg,MAX_MSG_LEN,&i);
		printf("%s\n",msg);
	}
	for(int j=0;j<6;j++){
		printf("%d ",flag[j]);
	}
	printf("\n");
	pmq_close(mq);
	return ;
}
