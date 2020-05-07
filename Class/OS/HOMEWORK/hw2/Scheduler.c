#include "Headers.h"

int RunScheduler( void )
{
    printf("RunScheduler %d\n",getpid());
    alarm(TIMESLICE);
    //printf("%d 의 TIMESLICE 재정의\n",getpid());
}

void    __ContextSwitch(int curpid, int newpid){//인자를 pid 라고 가정
    // alarm(0);
    // printf("get %d /// ",getpid());
    
    //kill(getpid(),SIGUSR2);
    printf("%d -> %d \n",curpid,newpid);
    
    int nn = find_tid(newpid);
    int cc = find_tid(curpid);
    Thread * newT = pThreadTbEnt[nn].pThread;
    Thread * curT = pThreadTbEnt[cc].pThread;
    pCurrentThread = newT;
    newT->status = THREAD_STATUS_RUN;

    printf("ContextSwitching Success...\n");
        print_pThreadEnt();
        print_pWaitingQueue();
        print_pReadyQueue();
        print_pCurrentThread();
        printf(">>>>>>>>>>>>>>>>%d>>>>>>>>>>>>>>>>>>>\n\n",getpid());
    kill(newpid, SIGCONT);
    kill(mainPid,SIGUSR2);
    if(curT->status!=THREAD_STATUS_ZOMBIE || curT->status !=THREAD_STATUS_WAIT)
        kill(curpid, SIGSTOP);
    return;
}
