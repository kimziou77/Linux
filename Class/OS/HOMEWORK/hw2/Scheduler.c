#include "Headers.h"

int RunScheduler( void )
{
    // printf("RunScheduler %d\n",getpid());
    alarm(2);
    //printf("%d 의 TIMESLICE 재정의\n",getpid());
}

void    __ContextSwitch(int curpid, int newpid){//인자를 pid 라고 가정
    

    printf("CONTEXTSWITCHIGN %d \n",getpid());
    printf("%d -> %d \n",curpid,newpid);

    int nn = find_tid(newpid);
    int cc = find_tid(curpid);
    Thread * newT = pThreadTbEnt[nn].pThread;
    Thread * curT = pThreadTbEnt[cc].pThread;
    pCurrentThread = newT;
    newT->status = THREAD_STATUS_RUN;
    
    kill(curpid, SIGSTOP);
    
    print_all();
    // printf(">>>>>>>>>>>>>>>>%d>>>>>>>>>>>>>>>>>>>\n",getpid());

    // printf("끝\n\n");
    kill(newpid, SIGCONT);
    alarm(TIMESLICE);
    // kill(pThreadTbEnt[0].pThread->pid, SIGCONT);
    //signal(SIGCHLD,signalHandler);
    return;
}
