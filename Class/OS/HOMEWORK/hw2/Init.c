#include "Headers.h"

void Init(void)
{
    
    //Create ready queue and waiting queue
    // Initailize thread scheduler
    if(signal(SIGALRM, schedule)==SIG_ERR){
        perror("signal() error!");
    }
    // if(signal(SIGUSR1, wakeUp)==SIG_ERR){
    //     perror("signal() error!");
    // }
    if(signal(SIGUSR2, RunScheduler)==SIG_ERR){
        perror("signal() error!");
    }
    if(signal(SIGCHLD, signalHandler)==SIG_ERR){
        perror("signal() error!");
    }
    for(int i=0;i<MAX_THREAD_NUM;i++){//TCB 초기화
        pThreadTbEnt[i].bUsed   = FALSE;
        pThreadTbEnt[i].pThread = NULL;
    }

	for (int i = 0; i < MAX_READYQUEUE_NUM; i++) { //ReadyQueue초기화
		pReadyQueueEnt[i].pHead = NULL;
		pReadyQueueEnt[i].pTail = NULL;
        pReadyQueueEnt[i].queueCount=0;
	}
    //Waiting queue 초기화
    pWaitingQueueHead = NULL;
    pWaitingQueueTail = NULL;
}
void signalHandler(int signum){
    if(signum==SIGCHLD){
        printf("SIGCHLD@\n");
        pause();
    }
    else if(signum==SIGUSR2){
        printf("SIGUSR2@ ");
        RunScheduler();
    }
    
}
void schedule(int signum){
        alarm(0);
        //printf("\n\nScheduler실행(밑은 현재상태)\n");
        //ReadyQueue is empty?
        if(!IsReadyQueueEmpty()){//No
            
            Thread * nThread = GetThreadFromReadyQueue();//get new thread
            if(nThread->priority)
            DeleteThreadFromReadyQueue(nThread);//Remove TCB of new thread From ready queue

            InsertThreadToReadyQueue(pCurrentThread);//put running thread to the tail of ready queue
            pCurrentThread->status=THREAD_STATUS_READY;
            
            //printf("상태 변경 scheduler Context Switch ");
            __ContextSwitch(pCurrentThread->pid,nThread->pid);
        }
}
