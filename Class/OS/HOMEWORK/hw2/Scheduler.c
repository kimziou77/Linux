#include "Headers.h"

BOOL Is_Ready_Queue_Empty(){
    for(int i=0;i<MAX_READYQUEUE_NUM;i++){
        if(pReadyQueueEnt[i].queueCount != 0) return FALSE;
    }
    return TRUE;
}

int RunScheduler( void )
{//current 기준
    
    while(1){
         // keep doing 
         printf("RunScheduler\n");
        int pr= pCurrentThread->priority;
        if(pReadyQueueEnt[pr].queueCount!=0){ // round robin
            //pReadyQueueEnt[pr].pTail->phNext=pCurrentThead;
            //pReadyQueueEnt[pr].pTail = pCurrentThead;
            __ContextSwitch(pCurrentThread->pid,pReadyQueueEnt[pr].pHead->pid);
        }
        else{ // 다음 우선순위 round robin
            for(int i=0;i<MAX_READYQUEUE_NUM;i++){
                if(pReadyQueueEnt[i].queueCount!=0){
                    __ContextSwitch(pCurrentThread->pid,pReadyQueueEnt[i].pHead->pid);
                }
            }
        }
        alarm(TIMESLICE);
        pause();//pause를 써도 되나? 안될것같은데
    }
    
        //if(Is_Ready_Queue_Empty()){ //레디큐에 무언가 존재한다면??
        /*
        Priority based Round Robin
        running스레드를 readyQueue의 tail로 넣기.
        readyQueue에서 새로운 스레드TCB를 제거한다.
        그리고 contextSwitching 한다.
        __ContextSwitch(curpid, -- );
        */

}

void    __ContextSwitch(int curpid, int newpid){    //인자를 pid 라고 가정
    printf("ContextSwtiching %d -> %d \n",curpid,newpid);
    
    int nn = find_tid(newpid);
    int cc = find_tid(curpid);
    Thread * newT = pThreadTbEnt[nn].pThread;
    Thread * curT = pThreadTbEnt[cc].pThread;

    //TODO: 근데 얘가 레디에있 는지 웨이트에 있는지 어케알어 ㅇㅅㅇ
    DeleteThreadFromReadyQueue(newT);//레디큐에 없으면 실행안함
    //DeleteThreadFromReadyQueue(newT);//웨잇큐에 없으면 실행 안함
    InsertThreadToReadyQueue(curT);
    pCurrentThread = newT;

    print_pThreadEnt();
    print_pWaitingQueue();
    print_pReadyQueue();
    print_pCurrentThread();
    printf("\n\n");
    //printf("@\n");
    kill(newpid, SIGCONT);
    //printf("@@\n");
    kill(curpid, SIGSTOP);
    //printf("@@@\n");

    printf("ContextSwitching Success...\n");
    return;
}
