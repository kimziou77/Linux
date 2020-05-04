#include "Headers.h"

BOOL Is_Ready_Queue_Empty(){
    for(int i=0;i<MAX_READYQUEUE_NUM;i++){
        if(pReadyQueueEnt[i].queueCount != 0) return FALSE;
    }
    return TRUE;
}

int RunScheduler( void )
{//current 기준
    printf("RunScheduler\n");
    while(1){
         // keep doing 
        int pr= pCurrentThead->priority;
    
        if(pReadyQueueEnt[pr].queueCount!=0){ // round robin
            printf("round - robin\n");
            //pReadyQueueEnt[pr].pTail->phNext=pCurrentThead;
            //pReadyQueueEnt[pr].pTail = pCurrentThead;
            pReadyQueueEnt[pr].queueCount++;
            printf("queueCount : %d\n",pReadyQueueEnt[pr].queueCount);

            DeleteThreadFromReadyQueue(pReadyQueueEnt[pr].pHead);
            pReadyQueueEnt[pr].pHead = pReadyQueueEnt[pr].pHead->phNext;
            __ContextSwitch(pCurrentThead,pReadyQueueEnt[pr].pHead);
        }
        else{ // 다음 우선순위 round robin
            printf("nextPriority round - robin\n");
            int flag = 1;
            for(int i=0;i<MAX_READYQUEUE_NUM;i++){
                if(pReadyQueueEnt[i].queueCount!=0){
                    flag=0;
                    pReadyQueueEnt[i].pTail->phNext=pCurrentThead;
                    pReadyQueueEnt[i].pTail = pCurrentThead;
                    pReadyQueueEnt[i].queueCount++;
                    DeleteThreadFromReadyQueue(pReadyQueueEnt[i].pHead);
                    pReadyQueueEnt[i].pHead = pReadyQueueEnt[i].pHead->phNext;
                    __ContextSwitch(pCurrentThead,pReadyQueueEnt[pr].pHead);
                }
            }
            //if(flag) break;
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

    //이걸 해주고 종료되면 알람이 오나?
    //여기서 멈춰놔야 하지 않을까?

    //주기적으로(?) 시그널을 발생시켜준다.
}

//systemcall로 생성을 했으므로
//systemcall을 이용해 contextSwitching을 할 것.

void    __ContextSwitch(int curpid, int newpid){
    //인자를 Tid 라고 가정
    printf("ContextSwtiching %d -> %d \n",curpid,newpid);
    print_pThreadEnt();
    int nn= find_tid(newpid);
    //printf("@\n");
    kill(newpid, SIGCONT);
    //printf("@@\n");
    kill(curpid, SIGSTOP);
    //printf("@@@\n");

    pCurrentThead = pThreadTbEnt[nn].pThread;
    printf("ContextSwitching Success...Maybe?\n");
    return;
}
