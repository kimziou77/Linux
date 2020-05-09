#include "Headers.h"

void Init(void)
{
    
    //Create ready queue and waiting queue
    // Initailize thread scheduler
    if(signal(SIGALRM, signalHandler)==SIG_ERR){
        perror("signal() error!");
    }
    if(signal(SIGUSR1, signalHandler)==SIG_ERR){
        perror("signal() error!");
    }
    if(signal(SIGUSR2, signalHandler)==SIG_ERR){
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
    if(signum == SIGALRM){
        printf("SIGALRM@ %d \n",getpid());
        schedule(signum);
    }
    else if(signum==SIGCHLD){
        RunScheduler();
        printf("SIGCHLD@ %d \n",getpid());
    }
    else if(signum==SIGUSR2){
        printf("SIGUSR2@ %d ",getpid());
        RunScheduler();
    }
    else if(signum==SIGUSR1){
        printf("SIGUSR1 @ %d\n",getpid());
        RunScheduler();
    }
}
void schedule(int signum){
        alarm(0);
        //printf("schedule %d \n",getpid());
        //printf("\n\nScheduler실행(밑은 현재상태)\n");
        //ReadyQueue is empty?
        if(!IsReadyQueueEmpty()){//No
            
            Thread * nThread = GetThreadFromReadyQueue();//get new thread
            if(nThread->priority < pCurrentThread->priority){
                DeleteThreadFromReadyQueue(nThread);//Remove TCB of new thread From ready queue
                InsertThreadToReadyQueue(pCurrentThread);//put running thread to the tail of ready queue
                pCurrentThread->status=THREAD_STATUS_READY;

                __ContextSwitch(pCurrentThread->pid,nThread->pid);
            }
           
        }
}
