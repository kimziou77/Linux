#include "Headers.h"

int RunScheduler( void )
{
    if(DEBUGGING) printf("[+] RunScheduler\n");
    alarm(TIMESLICE);
    if(DEBUGGING) printf("[-] RunScheduler\n");
}

void    __ContextSwitch(int curpid, int newpid){//인자를 pid 라고 가정
    if(DEBUGGING) printf("[+] ContextSwtich\n");
    if(DEBUGGING) printf("%d -> %d\n",curpid,newpid);
    if(DEBUGGING)   print_all();
    if(curpid)    kill(curpid,SIGSTOP);//0이 아니면 실행
    if(newpid)    kill(newpid,SIGCONT);
    
    if(DEBUGGING) printf("[-] ContextSwtich\n");
    return;
}
void schedule(int signum){
        if(DEBUGGING) printf("[+] scedule\n");
        
        Thread * nThread;
        //ReadyQueue is empty?
        if(!IsReadyQueueEmpty()){//No
            if(DEBUGGING) printf("ReadyQueue not empty\n");

            nThread = GetThreadFromReadyQueue();//get new thread

            if(pCurrentThread==NULL){
                if(DEBUGGING) printf("현재 실행중인 프로세스가 없음 ->누군가가 Reaping을 거치지 않고 종료되어버림(현재 스레드가 좀비스레드임)\n");
                DeleteThreadFromReadyQueue(nThread);

                pCurrentThread= nThread;
                nThread->status = THREAD_STATUS_RUN;
                kill(nThread->pid,SIGCONT);
            }
            else{
                if(DEBUGGING) printf("현재 실행중인 프로세스가 존재함 %d \n",pCurrentThread->pid);
                Thread * oThread = pCurrentThread;
                // printf("schedule : cur %d new %d\n", pCurrentThread->pid, nThread->pid);

                if(DEBUGGING) printf("현재 실행중인 프로세스가 존재함 nThread %d \n",nThread);

                if(nThread->priority <= pCurrentThread->priority){
                    if(DEBUGGING) printf("새 프로세스의 우선순위가 더 높거나, 같은 우선순위의 프로세스가 타임슬라이스가 다 되었음\n");
                    DeleteThreadFromReadyQueue(nThread);//Remove TCB of new thread From ready queue

                if(DEBUGGING) printf("현재 실행중인 프로세스가 존재함 DeleteThreadFromReadyQueue %d \n",nThread);
                    if(pCurrentThread->status != THREAD_STATUS_ZOMBIE){
                        if(DEBUGGING) printf("현재 프로세스가 좀비가 아님(exit을 하지 않음)\n");
                        
                        InsertThreadToReadyQueue(pCurrentThread);//put running thread to the tail of ready queue
                        oThread->status = THREAD_STATUS_READY;
                        pCurrentThread = nThread;
                        pCurrentThread->status=THREAD_STATUS_RUN;
                        __ContextSwitch(oThread->pid,nThread->pid);
                    }
                    else{
                        if(DEBUGGING)   printf("좀비인데 비 정상적인 루트임 수정 요망\n");
                        
                    }
                }  
            }
           
        }
        else{
            if(DEBUGGING) printf("ReadyQueue empty\n");
            if(DEBUGGING) print_all();

             kill(pCurrentThread->pid,SIGCONT);
        }
        alarm(TIMESLICE);
        
        if(DEBUGGING)   printf("[-] schedule\n");
}
