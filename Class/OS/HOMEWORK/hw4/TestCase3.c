#include "TestCase3.h"

void* Tc3ThreadPing(void* param)
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
		sprintf(msg,"%dth Ping",i);
		pmq_send(mq1,msg,MAX_MSG_LEN,0);
		memset(msg,0,MAX_MSG_LEN);
		pmq_receive(mq2,msg,MAX_MSG_LEN,&msg_prio);
		printf("%s\n",msg);
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

void* Tc3ThreadPong(void* param)
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
		printf("%s\n",msg);
		memset(msg,0,MAX_MSG_LEN);
		sprintf(msg,"%dth Pong",i);
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

void TestCase3(void)
{
	thread_t tid[2];
	pmqd_t mq;
	char msg[MAX_MSG_LEN];
	unsigned int i;
	int tmp1=0,tmp2=1;

	mq=pmq_open("mq1",O_CREAT,O_RDWR,NULL);

	thread_create(&tid[0], NULL, 1, (void*)Tc3ThreadPong,(void*)&tmp1);
	thread_create(&tid[1], NULL, 1, (void*)Tc3ThreadPing,(void*)&tmp2);

	memset(msg,0,MAX_MSG_LEN);
	pmq_receive(mq,msg,MAX_MSG_LEN,&i);
	printf("%s\n",msg);

	memset(msg,0,MAX_MSG_LEN);
	pmq_receive(mq,msg,MAX_MSG_LEN,&i);
	printf("%s\n",msg);


	pmq_close(mq);
	return ;
}
