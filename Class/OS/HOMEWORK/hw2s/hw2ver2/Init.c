#include "Headers.h"
#include <signal.h>

void Init(void)
{
    mainPid = getpid();
    struct sigaction sa;

    sa.sa_handler = signalHandler;
    sa.sa_flags = SA_SIGINFO|SA_NOCLDSTOP;//자식 정보, exit에서만 SIGCHLD
    
    //핸들러 등록
    if(sigaction(SIGALRM, &sa, 0)==SIG_ERR){
        perror("signal() error!");
    }
    if(sigaction(SIGUSR1, &sa, 0)==SIG_ERR){
        perror("signal() error!");
    }
    if(sigaction(SIGUSR2, &sa, 0)==SIG_ERR){
        perror("signal() error!");
    }
    if(sigaction(SIGCHLD, &sa, 0)==SIG_ERR){
        perror("signal() error!");
    }

    //Create ready queue and waiting queue
    // Initailize thread scheduler

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
void signalHandler(int signum, siginfo_t *info, void *context){
    alarm(0);
    if(DEBUGGING)   printf("[+] signalHandler %d child %d\n",signum, info->si_pid);
    if(pCurrentThread!=NULL)
    {
        if(DEBUGGING) printf("SignalHandler : pCurrentThread - %d \n", pCurrentThread->pid);
         kill(pCurrentThread->pid,SIGSTOP);
    }   
    
    
    

    if(signum == SIGALRM){//스케줄러 실행
        if(DEBUGGING)   printf("[@] SIGALRM %d \n",getpid());
        schedule(signum);
    }
    else if(signum==SIGCHLD){//pid가 exit하고, 부모스레드가 해당 pid를 리핑해줌.
        if(DEBUGGING)   printf("[@] SIGCHLD %d \n",getpid());
    }
    else if(signum==SIGUSR1){//다시 2초 예약
        if(DEBUGGING)   printf("[@] SIGUSR2 %d\n",getpid());
        RunScheduler();
    }
    else if(signum==SIGUSR2){//아직 사용하지않음
        if(DEBUGGING)   printf("[@] SIGUSR1 %d\n",getpid());
        
    }
    else{// Unknown signal
        if(DEBUGGING)   printf("[@] Handled!\n");
    }
    
    if(DEBUGGING)   printf("[-] signalHandler %d\n",signum);
}
