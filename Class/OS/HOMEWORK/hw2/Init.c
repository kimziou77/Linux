#include "Headers.h"

void * foo1(void* arg){
    printf("Hello I'm foo1\n");
    thread_exit(0);
    //while(1);
}
void * foo2(void* arg){
    printf("Hello I'm foo2\n");
    thread_exit(0);
    //while(1);
}
void * foo3(void* arg){
    printf("Hello I'm foo3\n");
    thread_exit(0);
    //while(1);
}
void TestCase(void){
    printf("TestCase Start!\n");
    //printf("pid : %d",thread_self());
    int tid1,tid2,tid3;
    int arg1,arg2,arg3;

    thread_create(&tid1,NULL,1,foo1,&arg1);
    thread_create(&tid2,NULL,2,foo2,&arg2);
    thread_create(&tid3,NULL,3,foo3,&arg3);
    
    //printf("%d %d %d\n",tid1,tid2,tid3);
    
    
    thread_suspend(tid3);
    thread_suspend(tid2);
    
    thread_resume(tid1);
    
    while(1);
}
void * AppTask(void* param)
{
    TestCase();
    return NULL;
}

void Init(void)
{
    //Create ready queue and waiting queue
    // Initailize thread scheduler
    if(signal(SIGALRM, signalHandler)==SIG_ERR){
        perror("signal() error!");
    }

    for(int i=0;i<MAX_THREAD_NUM;i++){//TCB 초기화
        pThreadTbEnt[i].bUsed   = FALSE;
        pThreadTbEnt[i].pThread = NULL;
    }

	for (int i = 0; i < MAX_READYQUEUE_NUM; i++) { //ReadyQueue초기화
		pReadyQueueEnt[i].pHead = NULL;
		pReadyQueueEnt[i].pTail = NULL;
        pReadyQueueEnt[i].queueCount=0;
	}
    //Waiting queue 초기화
    pWaitingQueueHead = NULL;
    pWaitingQueueTail = NULL;
}
void signalHandler(int signum){
    if(signum==SIGCHLD){
        printf("SIGCHLD\n");
    }
    else if(signum==SIGALRM){
        printf("SIGARLM\n");
    }
    else{
        printf("Handle\n");
    }
    //시그널 핸들러 안에서 무엇을 해야한다면
    //Priority based Round Robin을 해줘야함.
    //알람이 울리면 RunScheduler 실행해주기?
    
}
