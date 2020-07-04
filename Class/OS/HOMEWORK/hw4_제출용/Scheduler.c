#include "Headers.h"

int RunScheduler( void )
{
    alarm(TIMESLICE);
}

void    __ContextSwitch(int curpid, int newpid){
    
    // kill(mainPid,SIGUSR1);
    if(curpid)    kill(curpid,SIGSTOP);
    if(newpid)    kill(newpid,SIGCONT);
    
    return;
}
void schedule(int signum){
    
        Thread * nThread;
        //ReadyQueue is empty?
        if(!IsReadyQueueEmpty()){
            //No
    
            nThread = GetThreadFromReadyQueue();//get new thread

            if(pCurrentThread==NULL){
                DeleteThreadFromReadyQueue(nThread);
    
                pCurrentThread= nThread;
                nThread->status = THREAD_STATUS_RUN;
    
                __ContextSwitch(NONE,nThread->pid);
            }
            else if(pCurrentThread->status == THREAD_STATUS_WAIT){
    
                Thread * oThread = pCurrentThread;
                pCurrentThread= nThread;
                nThread->status = THREAD_STATUS_RUN;
                __ContextSwitch(oThread->pid,nThread->pid);
            }
            else{
                Thread * oThread = pCurrentThread;
    
                if(nThread->priority <= pCurrentThread->priority){
    
                    DeleteThreadFromReadyQueue(nThread);//Remove TCB of new thread From ready queue

                    if(pCurrentThread->status != THREAD_STATUS_ZOMBIE){
    
                        InsertThreadToReadyQueue(pCurrentThread);//put running thread to the tail of ready queue
                        oThread->status = THREAD_STATUS_READY;
                        pCurrentThread = nThread;
                        pCurrentThread->status=THREAD_STATUS_RUN;
                        __ContextSwitch(oThread->pid,nThread->pid);
                    }
                    else{
    
                        // printf("좀비인데 비 정상적인 루트임 수정 요망\n");
                    }
                }
                else{
    
                    kill(pCurrentThread->pid,SIGCONT);
                }
            }
           
        }
        else{
            // printf("ReadyQueue empty\n");
    
             kill(pCurrentThread->pid,SIGCONT);
        }
    
        alarm(TIMESLICE);
}
