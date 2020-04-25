#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"

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
    RunScheduler();
}
