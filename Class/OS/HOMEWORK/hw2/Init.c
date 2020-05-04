#include "Headers.h"
void alrmHandler(int signum);

void * foo1(void* arg){

    while(1);
}
void * foo2(void* arg){

    while(1);
}
void * foo3(void* arg){

    while(1);
}

void TestCase(void){
    int tid1,tid2,tid3;
    int arg1,arg2,arg3;

    thread_create(&tid1,NULL,0,foo1,&arg1);
    thread_create(&tid2,NULL,1,foo2,&arg2);
    thread_create(&tid3,NULL,2,foo3,&arg3);

    thread_suspend(tid1);
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
    if(signal(SIGALRM, alrmHandler)==SIG_ERR){
        perror("signal() error!");
    }

    for(int i=0;i<MAX_THREAD_NUM;i++){
        pThreadTbEnt[i].bUsed   = FALSE;
        pThreadTbEnt[i].pThread = NULL;
    }
}
void alrmHandler(int signum){
    //시그널 핸들러 안에서 무엇을 해야한다면
    //Priority based Round Robin을 해줘야함.
    //알람이 울리면 RunScheduler 실행해주기?
    printf("알람핸들러");
    RunScheduler();
}
