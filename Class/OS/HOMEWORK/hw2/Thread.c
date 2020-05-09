#include "Headers.h"


int thread_create(thread_t * thread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg)
{
    char* pStack;
    pStack= malloc(STACK_SIZE);
    int flags= SIGCHLD|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_VM;
    thread_t pid = clone(start_routine,(char*)pStack+STACK_SIZE,flags,arg); 
    kill(pid,SIGSTOP);

    printf("thread_Create %d\n",pid);
    
    //Allocate TCB
    Thread* childThread = (Thread*)malloc(sizeof(Thread));
    childThread->pid=pid;
    childThread->priority=priority;
    childThread->stackSize=STACK_SIZE;
    childThread->stackAddr=pStack;//TODO: 주소가 이게 맞나?
    childThread->exitCode = NON_EXIT;
    
    *thread = FindEmptyThreadTable();//빈 TCB table을 찾아 id 값 할당.
    pThreadTbEnt[*thread].pThread = childThread;


    //Child Priority > Priority of running thread?
    if(pCurrentThread == NULL){//현재 실행중인 스레드가 없다면
        childThread->status=THREAD_STATUS_RUN;//바로 실행.
        pCurrentThread = childThread;
        kill(pCurrentThread->pid,SIGCONT);
    }
    else if(childThread->priority >= pCurrentThread->priority){//YES >= ? >
        childThread->status=THREAD_STATUS_READY;//Set child Thread status to Ready
        InsertThreadToReadyQueue(childThread);//Move TCB to ready queue
    }
    else{//NO
        InsertThreadToReadyQueue(childThread);//Move TCB of running thread to ready queue
        pCurrentThread->status = THREAD_STATUS_READY;//Set the thread status to Ready
        childThread->status=THREAD_STATUS_RUN;//Set child thread status to running 
        //Context Switching from the running thread to child Thread
        printf("thread create ConTextSwitch ");
        __ContextSwitch(pCurrentThread->pid,childThread->pid);

    }
    return *thread;
}

int thread_suspend(thread_t tid){
    printf("%d suspend \n",tid);
    //자기 자신을 멈추는 동작은 non-precondition
    if(tid<0 || tid >= MAX_THREAD_NUM) return FAILED;//배열 인덱스초과 -> FAILED
    if(pThreadTbEnt[tid].bUsed==FALSE) return FAILED;//멈추려는 스레드가 존재하지 않으면 FAILED
    
    Thread * pThread = pThreadTbEnt[tid].pThread;//멈출 대상

    //Move TCB to waiting queue
    if(pThread->status==THREAD_STATUS_RUN){ //RUN
        // printf("suspend @ RUN\n");
        InsertThreadToWaitingQueue(pThread);//Move TCB to waiting queue
        pThread->status = THREAD_STATUS_WAIT;//Set status to waiting
        Thread * nThread = GetThreadFromReadyQueue();
        nThread->status = THREAD_STATUS_RUN;
        pCurrentThread = nThread;
        __ContextSwitch(pThread->pid,nThread->pid);//Context Switcing
    }
    else if(pThread->status==THREAD_STATUS_READY){  //READY
        // printf("suspend @@ READY\n");
        DeleteThreadFromReadyQueue(pThread);
        InsertThreadToWaitingQueue(pThread);//Move TCB to waiting queue
        pThread->status= THREAD_STATUS_WAIT;//Set thread status to waiting
    }
    else if(pThread->status==THREAD_STATUS_WAIT){// WAIT
        // printf("suspend @@@ WAIT");
        //noting happened
    }
    return SUCCESS;
}
//자기 자신을 종료하는것은 없다
int thread_cancel(thread_t tid)//해당 스레드를 terminate 시키는 함수
{
    
    Thread * targetThread = pThreadTbEnt[tid].pThread;
    kill(targetThread->pid,SIGKILL);//there is no self kill(non-precondition)
    // printf("Therad cancel %d \n",targetThread->pid);
        
    //TODO: exit 했는데 다시 킬해도 되나?
    //Remove TCB from ready or Waiting queue
    if(targetThread->status==THREAD_STATUS_READY){
        // printf("1\n");
        DeleteThreadFromReadyQueue(targetThread);
    }
    else if(targetThread->status==THREAD_STATUS_WAIT){
        // printf("2\n");
        DeleteThreadFromWaitingQueue(targetThread);
    }
    else{
        printf("Error : cancel Failed!\n");
        return FAILED;
    }
    targetThread->status =THREAD_STATUS_ZOMBIE;

    //  Deallocate tid's thread TCB
    // printf("할당 해제합니당\n");
    // print_all();
    pThreadTbEnt[tid].bUsed = FALSE;
    pThreadTbEnt[tid].pThread = NULL;
    free(targetThread->stackAddr);
    free(targetThread);

    return SUCCESS;
}

int thread_resume(thread_t tid)
//잠든 스레드(waiting Thread) 깨우는 함수 priority 에 따라 동작이 달라진다.
{
    if(tid<0 || tid >= MAX_THREAD_NUM) return FAILED;//인덱스초과 -> FAILED
    Thread* targetThread = pThreadTbEnt[tid].pThread;//resume 대상

    //Target prioirty > Priority of running thread?
    if(targetThread->priority > pCurrentThread->priority){//YES : target -> running
        //Move TCB of running thread to ready queue
        InsertThreadToReadyQueue(pCurrentThread);
        //Set the thread status to ready
        pCurrentThread->status = THREAD_STATUS_READY;

        //Remove TCB of target thread from waiting queue
        WaitingQueue_To_ReadyQueue(targetThread);
        //Set the thread status to ruuning
        targetThread->status=THREAD_STATUS_RUN;
        pCurrentThread = targetThread;
        //Context switching from the running thread to target thread
        // printf("thread resume ContextSwitch ");
        __ContextSwitch(pCurrentThread->pid, targetThread->pid);
    }
    else{//NO : target -> ready
        //Set target thread status to ready
        pCurrentThread->status=THREAD_STATUS_READY;
        //Move TCB from waiting queue to ready queue
        targetThread->status =THREAD_STATUS_READY;
        WaitingQueue_To_ReadyQueue(targetThread);
    }
    // print_all();

    return SUCCESS;//Return
}

thread_t thread_self()//tid 반환한다
{
    int pid= getpid();
    //printf("Thread Self : %d \n",pid);
    for(int i=0;i < MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].pThread->pid==pid){
            return i;// thread ID
        }            
    }
    printf("NONE ID \n");
}
void wakeUp(){
    // printf("wake up @ %d \n",getpid());

    Thread * prtThread = pThreadTbEnt[0].pThread;

    Thread * nThread = GetThreadFromReadyQueue();
    if(nThread!=NULL && prtThread->priority >= nThread->priority){
        // printf("1@@\n");
        //Remove new thread's TCB from ready queue
        DeleteThreadFromReadyQueue(nThread);
        //set new thread status to ruuning & Context Switching
        //printf("thread exit context switch ");
        nThread->status = THREAD_STATUS_RUN;
        pCurrentThread = nThread;

        // print_all();
        kill(nThread->pid, SIGCONT);
        //__ContextSwitch(pThread->pid,nThread->pid);
    }
    else if(nThread==NULL||prtThread->priority < nThread->priority){
        // printf("2@@\n");
        DeleteThreadFromWaitingQueue(prtThread); //Waiting Queue에서 제거
        pCurrentThread=prtThread;
        prtThread->status = THREAD_STATUS_RUN;
        // print_all();
        kill(prtThread->pid,SIGCONT);
        //
    }
    //시그널 핸들러 안에서 무엇을 해야한다면
    //Priority based Round Robin을 해줘야함.
}

int thread_join(thread_t tid, void * * retval){
    // printf("mainPid ; %d \n",mainPid);
    print_all();

    printf("thread_join child : %d를 기다릴거임 \n",pThreadTbEnt[tid].pThread->pid);
    thread_t pTid = thread_self();//TastCase 프로세스(부모)
    int pPid = pThreadTbEnt[pTid].pThread->pid;//TastCase 프로세스
    Thread * prtThread = pThreadTbEnt[pTid].pThread;//부모 스레드
    Thread * chdThread = pThreadTbEnt[tid].pThread;//자식 스레드

    if(chdThread->status != THREAD_STATUS_ZOMBIE){//Child status is ZOMBIE? -> No
        prtThread->status = THREAD_STATUS_WAIT;//Set this(Parent) Thread status to waiting
        InsertThreadToWaitingQueue(prtThread);//move this Thread's TCB to waiting queue
        Thread * nThread = GetThreadFromReadyQueue();//Select new Thread to run on CPU(scheduler);
        if(nThread!=NULL){
            DeleteThreadFromReadyQueue(nThread);//Remove new Thread's TCB from ready queue; 
            nThread->status = THREAD_STATUS_RUN;
            pCurrentThread = nThread;
            // kill(mainPid,SIGUSR2);
            // __ContextSwitch()
            
            // if(sighold(SIGCHLD)==0){
            //     printf("sighold succeess\n");
            // }
            // pause();//CONT신호 전송 -> 다른 신호 올 떄까지 blocking 자식이 부모한테로 보내는게 없으니까 멈춤상태로 계속있을 듯

            alarm(TIMESLICE);//TODO: 순서?
            kill(nThread->pid , SIGCONT);
            kill(mainPid, SIGUSR2);
            // pause();
            // if(sigrelse(SIGCHLD)==0){//그럼 차일드 받을때마다 얘가 일어나서 pause 를 안할텐데
            //     printf("Sigrelse succeess\n");
            // }
            // kill(getpid(),SIGCHLD);
            while(chdThread->status != THREAD_STATUS_ZOMBIE)    {
                // print_all();
                // printf("아직 좀비아님 다시 pause() \n");
                pause();
            }

        }
        //TODO: 여기서 컨텍스트 스위칭을 하면 STOP이 될텐데...? pasue로 어케하지
        //Set new Thread status to running & ContextSwitching to the new thread

        // kill(getpid(),SIGSTOP);//and waiting...나중에 child 가 종료가 되면 다음 문장 실행
        //SIGCHLD를 받고 다시 핸들러 실행 후 다음을 계속 실행한다.
    }
    // while(chdThread->status != THREAD_STATUS_ZOMBIE) pause();
    // printf("parent wake up !\n");
        //zombie reaping 작업 child Thread 청소
    //int chdTid = find_tid(chdThread->pid);
    // thread_cancel(chdTid);
    *retval = (int *)(&(chdThread->exitCode));//Get child's TCB Put exitCode into retVal
        // TODO: & 게 우선인가 -> 게 우선인가
    wakeUp();
        //xxxxDeleteThreadFromWaitingQueue(chdThread);xxx 이게 다이어그램에는 있는데 cancel에 포함되어있어서..
        //TODO: Remove child's TCB from waiting queue?
    // printf("retVal : %d\n",*((int *)retval));
        //thread_cancel(tid);
}
int thread_exit(void * retval){
    printf("exit\n");

    thread_t tid = find_tid(pCurrentThread->pid);//TODO: 필요한건가? 안필요한거같은데
    Thread * pThread = pCurrentThread;//Get this thread's TCB through pCurrentThread;

    pThread->exitCode = *((int*)retval);//Store exitCode to exitCode in this Thread's TCB
   
    //Move this thread's TCB to waiting queue
    InsertThreadToWaitingQueue(pThread);
    pThread->status = THREAD_STATUS_ZOMBIE;

    //스레드가 exit 하면 부모스레드에서 시그널을 받고 컨텍스트 스위치 아닌가?
    //Select new thread to run on CPU;

    //TODO: 이러면 자원할당은 어떻게 하지? SIGSTOP상태같은건가? 할당해제 안해줘도 되나?
    // printf("곧 exit 합니닷 : %d prtpid : %d \n",pThread->pid,getppid());
    kill(getppid(),SIGUSR1);
    pause();
    exit(0); // wakeup
    // exit(pThread->pid);TODO:
}

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
    printf("%d(%d:%d)",pCurrentThread->pid,pCurrentThread->priority,pCurrentThread->status);
    printf("\n----------------------------------\n");
}
thread_t find_tid(int pid){
    for(int i=0;i<MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].bUsed && pThreadTbEnt[i].pThread->pid==pid)
            return i;
    }
    printf("\n");
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
