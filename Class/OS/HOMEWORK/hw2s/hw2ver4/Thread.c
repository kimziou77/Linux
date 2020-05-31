#include "Headers.h"

int thread_create(thread_t * thread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg)
{
    char* pStack;
    pStack= malloc(STACK_SIZE);
    int flags= SIGCHLD|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_VM;
    thread_t pid = clone(start_routine,(char*)pStack+STACK_SIZE,flags,arg); 
    kill(pid,SIGSTOP);//생성 후 stop
    //Allocate TCB
    Thread* childThread = (Thread*)malloc(sizeof(Thread));
    childThread->pid=pid;
    childThread->priority=priority;
    childThread->stackSize=STACK_SIZE;
    childThread->stackAddr=pStack;
    childThread->exitCode = NON_EXIT;

    *thread = FindEmptyThreadTable();//빈 TCB table을 찾아 id 값 할당.
    pThreadTblEnt[*thread].pThread = childThread;

    if(pCurrentThread == NULL){//현재 실행중인 스레드가 없다면
        childThread->status=THREAD_STATUS_RUN;//바로 실행.
        pCurrentThread = childThread;
        __ContextSwitch(NONE,childThread->pid);
    }
    else if(childThread->priority >= pCurrentThread->priority){
        InsertThreadToReadyQueue(childThread);//Move TCB to ready queue
        childThread->status=THREAD_STATUS_READY;//Set child Thread status to Ready
    }
    else{
        InsertThreadToReadyQueue(pCurrentThread);
        pCurrentThread->status = THREAD_STATUS_READY;
        __ContextSwitch(pCurrentThread->pid,childThread->pid);
    }
    return *thread;// 생성한 프로세스의 tid값 반환
}

int thread_suspend(thread_t tid){
    if(pThreadTblEnt[tid].bUsed==FALSE) return FAILED;//멈추려는 스레드가 존재하지 않으면 FAILED
    Thread * pThread = pThreadTblEnt[tid].pThread;//멈출 대상
        if(pThread->status==THREAD_STATUS_RUN){ //RUN
        InsertThreadToWaitingQueue(pThread);//Move TCB to waiting queue
        pThread->status = THREAD_STATUS_WAIT;//Set status to waiting
        kill(mainPid,SIGALRM);
    }
    else if(pThread->status==THREAD_STATUS_READY){  //READY
        DeleteThreadFromReadyQueue(pThread);
        InsertThreadToWaitingQueue(pThread);//Move TCB to waiting queue
        pThread->status= THREAD_STATUS_WAIT;//Set thread status to waiting
    }
    else if(pThread->status==THREAD_STATUS_WAIT){// WAIT
        // printf("suspend @@@ WAIT");
    }
    
    return SUCCESS;
}
int thread_cancel(thread_t tid)//해당 스레드를 terminate 시키는 함수
{
    Thread * targetThread = pThreadTblEnt[tid].pThread;
    if(targetThread == NULL){
        return FAILED;
    }

    kill(targetThread->pid, SIGKILL);
    if(targetThread->status == THREAD_STATUS_READY){
        DeleteThreadFromReadyQueue(targetThread);
    }
    else if(targetThread->status == THREAD_STATUS_WAIT){
        DeleteThreadFromWaitingQueue(targetThread);
    }
    else{
        return FAILED;
    }
    targetThread->status = THREAD_STATUS_ZOMBIE;
    pThreadTblEnt[tid].bUsed = FALSE;
    pThreadTblEnt[tid].pThread = NULL;
    free(targetThread->stackAddr);
    free(targetThread);

    return SUCCESS;
}

int thread_resume(thread_t tid)
{

    Thread* targetThread = pThreadTblEnt[tid].pThread;

     if(targetThread->priority < pCurrentThread->priority){//resume대상이 더 우선순위가 높으면
        InsertThreadToReadyQueue(pCurrentThread);
        pCurrentThread->status = THREAD_STATUS_READY;
        WaitingQueue_To_ReadyQueue(targetThread);
        targetThread->status=THREAD_STATUS_READY;
        kill(mainPid,SIGALRM);//둘 다 레디큐에 넣고 메인에게 컨텍스트요청
    }
    else{
        WaitingQueue_To_ReadyQueue(targetThread);
    }

    // print_all();

    return SUCCESS;
}

thread_t thread_self()
{
    int pid= getpid();
    for(int i=0;i < MAX_THREAD_NUM;i++){
        if(pThreadTblEnt[i].pThread!=NULL && pThreadTblEnt[i].pThread->pid==pid){
            return i;// thread ID
        }
    }
    // printf("NONE ID \n");
}

int thread_join(thread_t tid, void * * retval){
    thread_t pTid = thread_self();
    Thread * pThread = pThreadTblEnt[pTid].pThread;
    Thread * cThread = pThreadTblEnt[tid].pThread;
    if(cThread->status != THREAD_STATUS_ZOMBIE){
        pThread->status = THREAD_STATUS_WAIT;
        InsertThreadToWaitingQueue(pThread);
        pCurrentThread = NULL;

        Thread * nThread = GetThreadFromReadyQueue();
        if(nThread!=NULL){
            DeleteThreadFromReadyQueue(nThread);
            nThread->status = THREAD_STATUS_RUN;
            pCurrentThread = nThread;//부모 -> new프로세스
            kill(mainPid, SIGALRM);//부모한테 컨텍스트스위칭 요청

            while(cThread->status != THREAD_STATUS_ZOMBIE) pause();
        }
        else{
            // printf("가져올 프로세스가 존재하지 않습니다.\n");
        }
    }
    else{
        // printf("자식프로세스가 이미 종료되었습니다.\n");
    }

    *retval = (int *)(&(cThread->exitCode));//Get child's TCB Put exitCode into retVal
    Reaping(cThread->pid);

    return SUCCESS;
  
}
void Reaping(int cPid){
    int pTid = thread_self();
    int cTid = find_tid(cPid);
    Thread * pThread = pThreadTblEnt[pTid].pThread;
    Thread * cThread = pThreadTblEnt[cTid].pThread;
    DeleteThreadFromWaitingQueue(pThread);

    if(pThread != pCurrentThread)
    {
        if(cThread==NULL){
            // printf("child Thread NULL\n");
            return;
        }
        //현재 스레드랑 비교
        if(pCurrentThread == NULL){
            InsertThreadToReadyQueue(pThread);
            pCurrentThread = NULL;
            kill(mainPid,SIGALRM);
            kill(getpid(), SIGSTOP);
        }
        else if(pThread->priority < pCurrentThread->priority){

            InsertThreadToReadyQueue(pThread);
            pCurrentThread = NULL;
            kill(mainPid,SIGALRM);
            kill(getpid(), SIGSTOP);
        }
        else{
            InsertThreadToReadyQueue(pThread);
        }
    }
    //reaping

    DeleteThreadFromWaitingQueue(cThread);
    pThreadTblEnt[cTid].bUsed = FALSE;
    pThreadTblEnt[cTid].pThread = NULL;
    free(cThread->stackAddr);
    free(cThread);
}
int thread_exit(void * retval){

   Thread * eThread = pCurrentThread;
    eThread->exitCode = *((int*)retval);
    InsertThreadToWaitingQueue(eThread);
    eThread -> status = THREAD_STATUS_ZOMBIE;
    pCurrentThread = NULL;
   exit(0);
}
