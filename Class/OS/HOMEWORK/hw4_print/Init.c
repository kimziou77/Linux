#include "Headers.h"
#include <signal.h>

void Init(void)
{
    mainPid = getpid();
    struct sigaction sa;

    sa.sa_handler = signalHandler;
    sa.sa_flags = SA_SIGINFO|SA_NOCLDSTOP;//정보, exit에서만 SIGCHLD
    
    //핸들러 등록
    if(sigaction(SIGALRM, &sa, NULL) < 0){
        perror("signal() error!");
    }
    if(sigaction(SIGUSR1, &sa, NULL) < 0){
        perror("signal() error!");
    }
    if(sigaction(SIGCHLD, &sa, NULL) < 0){
        perror("signal() error!");
    }

    //Create ready queue and waiting queue
    // Initailize thread scheduler

    for(int i=0;i<MAX_THREAD_NUM;i++){//TCB 초기화
        pThreadTblEnt[i].bUsed   = FALSE;
        pThreadTblEnt[i].pThread = NULL;
    }

	for (int i = 0; i < MAX_READYQUEUE_NUM; i++) { //ReadyQueue초기화
		pReadyQueueEnt[i].pHead = NULL;
		pReadyQueueEnt[i].pTail = NULL;
        pReadyQueueEnt[i].queueCount=0;
	}
    //Waiting queue 초기화
    pWaitingQueueHead = NULL;
    pWaitingQueueTail = NULL;
    for(int i=0;i < MAX_QCB_NUM ;i++){
        qcbTblEntry[i].bUsed=FALSE;
        qcbTblEntry[i].pQcb=NULL;
    }
}
void signalHandler(int signum, siginfo_t *info, void *context){
    alarm(0);
    if(DEBUGGING)   printf("[+] signalHandler %d child %d Me %d\n",signum, info->si_pid,getpid());
    if(pCurrentThread!=NULL)
    {
        if(DEBUGGING) printf("SignalHandler : pCurrentThread - %d \n", pCurrentThread->pid);
         kill(pCurrentThread->pid,SIGSTOP);
    }   
    if(signum == SIGALRM){//스케줄러 실행
        if(DEBUGGING) printf("[@] SIGALRM\n");
        schedule(signum);
    }
    else if(signum==SIGCHLD){
        //신호를 받기만한다. 부모가 일어난다.
    }
    else if(signum==SIGUSR1){//다시 2초 예약
        RunScheduler();
    }
    else{// Unknown signal
        
    }
}
