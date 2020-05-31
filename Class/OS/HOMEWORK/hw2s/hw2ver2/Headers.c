#include "Headers.h"
void print_pThreadEnt(){
    printf("-------pThreadEnt---------------\n");
    for(int i=0;i<MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].bUsed)
            printf("%d(%d:%d) ",pThreadTbEnt[i].pThread->pid,pThreadTbEnt[i].pThread->priority,pThreadTbEnt[i].pThread->status);
    }
    printf("\n------------------------------\n");
}
void print_pReadyQueue(){
    printf("-------pReadyQueue------------\n");
    for(int i=0;i<MAX_READYQUEUE_NUM;i++){
        if(pReadyQueueEnt[i].queueCount==0) continue;
        Thread* t = pReadyQueueEnt[i].pHead;
        
        if(t == NULL) continue;
        
        // printf("queueCount %d \n",pReadyQueueEnt[i].queueCount);
        for(int j=0; j < pReadyQueueEnt[i].queueCount;j++){
        
            printf("%d(%d:%d) ",t->pid,t->priority,t->status);
            t = t->phNext;
        }
    }
    printf("\n------------------------------\n");
}
void print_pWaitingQueue(){
    printf("-------pWaitingQueue------------\n");
    Thread * t = pWaitingQueueHead;
    while(t){
        printf("%d(%d:%d) ",t->pid,t->priority,t->status);
        if(t==t->phNext) break;
        t = t->phNext;

    }
    printf("\n------------------------------\n");
}
void print_pCurrentThread(){
    printf("-------pCurrentThread---------------\n");
    if(pCurrentThread != NULL)
        printf("%d(%d:%d)",pCurrentThread->pid,pCurrentThread->priority,pCurrentThread->status);
    printf("\n----------------------------------\n");
}

void print_all(){
        print_pThreadEnt();
        print_pWaitingQueue();
        print_pReadyQueue();
        print_pCurrentThread();
}
int FindEmptyThreadTable()
{
    for(int i=0;i<MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].bUsed==FALSE){
            pThreadTbEnt[i].bUsed=TRUE;
            return i;// thread ID
        }
    }
}
thread_t find_tid(int pid){
    for(int i=0;i<MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].bUsed && pThreadTbEnt[i].pThread->pid==pid)
            return i;
    }
    printf("\n");
}
