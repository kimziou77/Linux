#include "Headers.h"

int RunScheduler( void )
{
    printf("RunScheduler\n");
    alarm(1.8);
    //printf("%d 의 TIMESLICE 재정의\n",getpid());
}

void    __ContextSwitch(int curpid, int newpid){//인자를 pid 라고 가정
    // alarm(0);
    // printf("get %d /// ",getpid());
    printf("CONTEXTSWITCHIGN %d \n",getpid());
    kill(mainPid,SIGUSR2);
    
    //kill(getpid(),SIGUSR2);
    printf("%d -> %d \n",curpid,newpid);
    
    int nn = find_tid(newpid);
    int cc = find_tid(curpid);
    Thread * newT = pThreadTbEnt[nn].pThread;
    Thread * curT = pThreadTbEnt[cc].pThread;
    pCurrentThread = newT;
    newT->status = THREAD_STATUS_RUN;
    
    if(curT->status!=THREAD_STATUS_RUN){    
        //printf("%d sigstop, %d sigcont\n",curpid,newpid);
        kill(curpid, SIGSTOP);
    }
    else{
        printf("좀비스레드로 인한 컨텍스트 스위칭\n");
    }
    // printf("ContextSwitching Success...\n");
        print_pThreadEnt();
        print_pWaitingQueue();
        print_pReadyQueue();
        print_pCurrentThread();
    printf(">>>>>>>>>>>>>>>>%d>>>>>>>>>>>>>>>>>>>\n\n",getpid());

    //printf("%d sigcont\n",newpid);
    // printf("status %d \n",curT->status);

    kill(newpid, SIGCONT);
    printf("끝\n");
    return;
}
