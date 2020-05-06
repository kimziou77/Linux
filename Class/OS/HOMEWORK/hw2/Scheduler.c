#include "Headers.h"

int RunScheduler( void )
{
    printf("RunScheduler\n");
    alarm(0);
    alarm(TIMESLICE);
}

void    __ContextSwitch(int curpid, int newpid){//인자를 pid 라고 가정
    printf(" %d -> %d \n",curpid,newpid);
    
    int nn = find_tid(newpid);
    int cc = find_tid(curpid);
    Thread * newT = pThreadTbEnt[nn].pThread;
    Thread * curT = pThreadTbEnt[cc].pThread;
    pCurrentThread = newT;
    newT->status = THREAD_STATUS_RUN;

    printf("ContextSwitching Success...\n");
    kill(curpid, SIGSTOP);
    kill(newpid, SIGCONT);
    //이 함수를 실행하는 parent는 항상 main의 RunScheduler이어야 함
    //그래야 curpid를 스탑해도 Context Switching Success가 출력될것임
    
    return;
}
