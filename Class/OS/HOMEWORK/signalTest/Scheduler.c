#include "Headers.h"

int RunScheduler( void )
{
    // printf("RunScheduler\n");
    alarm(1.8);
    //printf("%d 의 TIMESLICE 재정의\n",getpid());
}

void    __ContextSwitch(int curpid, int newpid){//인자를 pid 라고 가정
    signal(SIGCHLD,SIG_IGN);
    //kill(mainPid, SIGUSR2);
    printf("CONTEXTSWITCHIGN %d \n",getpid());
    printf("%d -> %d \n",curpid,newpid);
    int nn = find_tid(newpid);
    int cc = find_tid(curpid);
    Thread * newT = pThreadTbEnt[nn].pThread;
    Thread * curT = pThreadTbEnt[cc].pThread;
    pCurrentThread = newT;
    newT->status = THREAD_STATUS_RUN;

    //kill(pThreadTbEnt[0].pThread->pid,SIGSTOP);
    
    kill(curpid, SIGSTOP);
    
    print_all();
    printf(">>>>>>>>>>>>>>>>%d>>>>>>>>>>>>>>>>>>>\n\n",getpid());

    printf("끝\n");
    kill(newpid, SIGCONT);
    // kill(pThreadTbEnt[0].pThread->pid, SIGCONT);
    signal(SIGCHLD,signalHandler);
    return;
}
