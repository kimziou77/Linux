#include "Headers.h"

BOOL Is_Ready_Queue_Empty(){
    for(int i=0;i<MAX_READYQUEUE_NUM;i++){
        if(pReadyQueueEnt[i].queueCount != 0) return FALSE;
    }
    return TRUE;
}

int RunScheduler( void )
{
    alarm(TIMESLICE);
}

void    __ContextSwitch(int curpid, int newpid){    //인자를 pid 라고 가정
    printf("ContextSwtiching %d -> %d \n",curpid,newpid);
    
    int nn = find_tid(newpid);
    int cc = find_tid(curpid);
    Thread * newT = pThreadTbEnt[nn].pThread;
    Thread * curT = pThreadTbEnt[cc].pThread;
    
    //TODO: 근데 얘가 레디에있 는지 웨이트에 있는지 어케알어 ㅇㅅㅇ
    //DeleteThreadFromReadyQueue(newT);//레디큐에 없으면 실행안함
    ////DeleteThreadFromReadyQueue(newT);//웨잇큐에 없으면 실행 안함
    //InsertThreadToReadyQueue(curT);
    
    pCurrentThread = newT;
    newT->status = THREAD_STATUS_RUN;
    print_pThreadEnt();
    print_pWaitingQueue();
    print_pReadyQueue();
    print_pCurrentThread();
    printf("\n\n");
    
    kill(newpid, SIGCONT);
    kill(curpid, SIGSTOP);

    printf("ContextSwitching Success...\n");
    return;
}
