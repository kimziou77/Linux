#include "Headers.h"

int RunScheduler( void )
{
    alarm(TIMESLICE);
}

void    __ContextSwitch(int curpid, int newpid){
    if(DEBUGGING) printf("[+] contextSwitch %d\n",getpid());
    // kill(mainPid,SIGUSR1);
    if(curpid)    kill(curpid,SIGSTOP);
    if(newpid)    kill(newpid,SIGCONT);
    if(DEBUGGING) print_all();
    if(DEBUGGING) printf("[-] contextSwitch\n");
    return;
}
void schedule(int signum){
        if(DEBUGGING) printf("[+] scehdule \n");
        Thread * nThread;
        //ReadyQueue is empty?
        if(!IsReadyQueueEmpty()){
            //No
            if(DEBUGGING) printf("레디큐에 무언가 존재 \n");
            nThread = GetThreadFromReadyQueue();//get new thread

            if(pCurrentThread==NULL){
                DeleteThreadFromReadyQueue(nThread);
                if(DEBUGGING) printf("현재 실행중인 스레드가 없음 새 스레드 돌린다\n");
                pCurrentThread= nThread;
                nThread->status = THREAD_STATUS_RUN;
                if(DEBUGGING) print_all();
                __ContextSwitch(NONE,nThread->pid);
            }
            else if(pCurrentThread->status == THREAD_STATUS_WAIT){
                if(DEBUGGING) printf("현재 스레드가 wait상태로 전환됨, 새 스레드 가져오기\n");
                Thread * oThread = pCurrentThread;
                pCurrentThread= nThread;
                nThread->status = THREAD_STATUS_RUN;
                __ContextSwitch(oThread->pid,nThread->pid);
            }
            else{
                Thread * oThread = pCurrentThread;
                if(DEBUGGING) printf("현재 실행중인 스레드가 존재\n");
                if(nThread->priority <= pCurrentThread->priority){
                    if(DEBUGGING) printf("새로운 애가 더 우선순위가 높음\n");
                    DeleteThreadFromReadyQueue(nThread);//Remove TCB of new thread From ready queue

                    if(pCurrentThread->status != THREAD_STATUS_ZOMBIE){
                        if(DEBUGGING) printf("현재 스레드가 좀비상태가 아님\n");
                        InsertThreadToReadyQueue(pCurrentThread);//put running thread to the tail of ready queue
                        oThread->status = THREAD_STATUS_READY;
                        pCurrentThread = nThread;
                        pCurrentThread->status=THREAD_STATUS_RUN;
                        __ContextSwitch(oThread->pid,nThread->pid);
                    }
                    else{
                        if(DEBUGGING) printf("현재 스레드가 좀비상태!!!\n");
                        // printf("좀비인데 비 정상적인 루트임 수정 요망\n");
                    }
                }
                else{
                    if(DEBUGGING) printf("현재 스레드가 우선순위가 더 높음 계속진행\n");
                    kill(pCurrentThread->pid,SIGCONT);
                }
            }
           
        }
        else{
            // printf("ReadyQueue empty\n");
            // print_all();
            if(DEBUGGING) printf("레디큐에 아무것도 없음 현재실행중인거 계속 실행\n");
             kill(pCurrentThread->pid,SIGCONT);
        }
        if(DEBUGGING) print_all();
        if(DEBUGGING) printf("[-] scehdule \n");
        alarm(TIMESLICE);
}
