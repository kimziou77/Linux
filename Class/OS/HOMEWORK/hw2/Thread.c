#include "Headers.h"


int thread_create(thread_t * thread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg)
{
    char* pStack;
    pStack= malloc(STACK_SIZE);

    int flags= SIGCHLD|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_VM;

    thread_t pid = clone(start_routine,(char*)pStack+STACK_SIZE,flags,arg); 
    printf("thread_Create %d\n",pid);
    kill(pid,SIGSTOP);//클론으로 실행된 스래드를 강제정지시킨다.

    //TODO:  TCB할당 -> deallocate 위치 확인할것
    Thread* pNewThread = (Thread*)malloc(sizeof(Thread));//TCB 하나 만들어서 clone 한 스레드를 관리한다?
    pNewThread->pid=pid;
    pNewThread->priority=priority;
    pNewThread->stackSize=STACK_SIZE;    
    //pNewTCB->stackAddr=

    *thread = FindEmptyThreadTable();//thread_id 빈 TCB table을 찾아 id 값 할당해줌.
    pThreadTbEnt[*thread].pThread = pNewThread;
    //branch..//실행중인 TCB랑 새로 생성된 TCB의 우선순위를 비교한다.
    if(pCurrentThread == NULL){//현재 실행중인 스레드가 없다면
        pNewThread->status=THREAD_STATUS_RUN;//바로 실행해준다.
        pCurrentThread = pNewThread;
        kill(pCurrentThread->pid,SIGCONT);//중지했던 스레드 다시 실행
    }
    else if(pCurrentThread->priority < pNewThread->priority){//현재실행중인 스레드가 더 우선순위 높으면
        pNewThread->status=THREAD_STATUS_READY;
        InsertThreadToReadyQueue(pNewThread);//레디큐에 넣는다.
    }
    else{//생성된 스레드가 우선순위가 더 높다면
        
        pCurrentThread->status = THREAD_STATUS_READY;
        InsertThreadToReadyQueue(pCurrentThread);//실행 스레드를 레디큐로
        pNewThread->status=THREAD_STATUS_RUN;
        DeleteThreadFromReadyQueue(pNewThread);//새 스레드를 레디큐에서 빼기
        
        __ContextSwitch(pCurrentThread->pid,pNewThread->pid);
    }
    return thread;
}

int thread_suspend(thread_t tid)
// tid 일시정지시키는 함수 wiating queue의 tail로 들어가게 되는데
// 이미 ready queue 또는 waiting queue 에 들어가 있을 수도 있다.
{
    //자기 자신을 멈추는 동작은 non-precondition
    if(tid<0 || tid >= MAX_THREAD_NUM) return FAILED;//배열 인덱스초과 -> FAILED

    if(pThreadTbEnt[tid].bUsed==FALSE) return FAILED;//멈추려는 스레드가 존재하지 않으면 FAILED
    
    Thread * pThread = pThreadTbEnt[tid].pThread;

    if(pThread->status==THREAD_STATUS_RUN){
        printf("suspend @ RUN\n");
        //waiting queue의 tail로
        InsertThreadToWaitingQueue(pThread);
        pThread->status = THREAD_STATUS_WAIT;
        Thread * nThread = GetThreadFromReadyQueue();
        __ContextSwitch(pThread->pid,nThread->pid);//레디큐 에 마지막애로 CS
        //Waiting Queue의 Tail에 넣는다.
    }
    else if(pThread->status==THREAD_STATUS_READY){
        printf("suspend @@ READY\n");
        // Ready 에 있는 스레드를 Wait으로 보내겠음

        DeleteThreadFromReadyQueue(pThread);
        InsertThreadToWaitingQueue(pThread);
        pThread->status= THREAD_STATUS_WAIT;
    }
    else if(pThread->status==THREAD_STATUS_WAIT){
        printf("suspend @@@ WAIT");
        //noting happened
    }
    
    return SUCCESS;
}

int thread_cancel(thread_t tid)//해당 스레드를 terminate 시키는 함수 ,there is no self kill(non-precondition)
{
    int pid= getpid();
    if(pThreadTbEnt[tid].pThread->pid==pid) return FAILED;//self kill 차단?

    kill(pThreadTbEnt[tid].pThread->pid,SIGKILL);//캔슬
    //  TCB를 레디, 웨이팅 큐 에서 뺴낸다.

    //  Deallocate tid's thread TCB
    pThreadTbEnt[tid].bUsed=FALSE;
    free(pThreadTbEnt[tid].pThread);
    //TODO: 이것만 해도 되나?

    return SUCCESS;
}

int thread_resume(thread_t tid)
//잠든 스레드(waiting Thread) 깨우는 함수, 조건에따라 running / ready 둘 다 갈 수 있다. 
{
    if(tid<0 || tid >= MAX_THREAD_NUM) return FAILED;//인덱스초과 -> FAILED

    //깨우는 대상의 priority 에 따라 동작이 달라진다.
    Thread* targetTCB = pThreadTbEnt[tid].pThread;//TCB로부터 우선순위를 받아온다.
    if(pCurrentThread->priority < targetTCB->priority){
        //우선순위 작으면 ready queue로 보낸다.
        targetTCB->status=THREAD_STATUS_READY;
        WaitingQueue_To_ReadyQueue(targetTCB);//waiting -> readyQueue
    }
    else{
        //현재 실행중인 스레드를 레디상태로
        pCurrentThread->status=THREAD_STATUS_READY;
        InsertThreadToReadyQueue(pCurrentThread);
        
        //새로 깨어나는 스레드를 러닝상태로
        targetTCB->status=THREAD_STATUS_RUN;
        DeleteThreadFromReadyQueue(targetTCB);
        
        //TODO: waiting queue 에 있던 TCB를 꺼내서 지우기 (run상태로 바꿀꺼니까 waiting queue에는 있을 필요가 없음.)

        __ContextSwitch(pCurrentThread->pid,targetTCB->pid);//TODO: 이 순서가 맞는지도 확인할것

    }
    return SUCCESS;
}

thread_t thread_self()//tid 반환한다
{
    int pid= getpid();
    printf("Thread Self : %d \n",pid);
    for(int i=0;i < MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].pThread->pid==pid){
            return i;// thread ID
        }            
    }
    printf("NONE ID \n");
}

int thread_join(thread_t tid, void ** retval){
    thread_t pTid = thread_self();
    int pPid = pThreadTbEnt[pTid].pThread->pid;
    printf("thread_join child : %d를 기다릴거임 \n",pThreadTbEnt[tid].pThread->pid);

    Thread * chdThread = pThreadTbEnt[tid].pThread;//얘가 자식 Thread인거 같은데
    Thread * prtThread = pThreadTbEnt[pTid].pThread;
    //Thread  *pThread;
    if(chdThread->status!=THREAD_STATUS_ZOMBIE){//아직 child가 종료되지 않았을 때
        prtThread->status = THREAD_STATUS_WAIT;//child를 기다리게 된다.
        InsertThreadToWaitingQueue(prtThread);//waiting queue로 보내주고

        Thread * nThread = GetThreadFromReadyQueue();//대신 일할 스레드 갖고오기
        DeleteThreadFromReadyQueue(nThread);
        //TODO: 여기서 컨텍스트 스위칭을 하면 STOP이 될텐데...? pasue로 어케하지

        __ContextSwitch(pCurrentThread->pid, nThread->pid);
        pause();//시그 차일듣 하면 다시 시그 컨트 해야하는건가? 퍼스인데ㅔ... 어쩌지
        
        printf("parent wake up !\n");
        /*이제 Parent가 깨어났음 -> child thread가 종료되었음.
            : SIGCHLD시그널이 child스레드로부터 생성되었음.
              Signal handler 구현 밑에서부터 코드가 시작되도록 구현해야한다.*/
        DeleteThreadFromWaitingQueue(prtThread);
        if(prtThread->priority < pCurrentThread->priority){
            __ContextSwitch(prtThread->pid, pCurrentThread->pid);// 1. 만약 여기서 현재실행중인  CPU보다 우선순위가 높다면 바로 running으로 간다.
        }
        else{
            InsertThreadToReadyQueue(prtThread);//2. 우선순위가 낮다면 ready 상태로 변경
        }
    }
        //zombie reaping 작업 child Thread 청소
        retval = chdThread->exitCode;//1. child의 TCB안에 있는 exitCode 를 retVal에 할당해준다.
        chdThread->pid;
        DeleteThreadFromWaitingQueue(chdThread);// 2. Child의 TCB를 waiting queue에서 지우고
        //TODO: 근데 얘가 waiting 하고있었나?
        free(chdThread);// 3. child  TCB를 할당해제한다.
    
}
int thread_exit(void * retval){
    printf("exit\n");
    int t = find_tid(pCurrentThread->pid);
    Thread * pThread = pThreadTbEnt[t].pThread;//현재 실행중인스레드

    pThread->exitCode = (int*)retval;
    //TODO: signal보내기

    //Ready queue에서 새거 하나 꺼내서 실행하기
    Thread * nThread = GetThreadFromReadyQueue();
    if(nThread!=NULL){//새거가 있다면
        DeleteThreadFromReadyQueue(nThread);//컨텍스트 스위칭
        InsertThreadToWaitingQueue(pThread);
        pThread->status = THREAD_STATUS_ZOMBIE;
        __ContextSwitch(pThread->pid,nThread->pid);
    }
    exit(pThread->pid);
    //현재 실행중인것은 정상종료 했고, 좀비상태로 웨이팅 큐에 넣기
    
    //TODO: 어떻게하면 exit을 호출할 수 있지..?exit();
     //exit(pCurrentThread->pid);
}
void print_pThreadEnt(){
    printf("-------pThreadEnt----------------\n");
    for(int i=0;i<MAX_THREAD_NUM && pThreadTbEnt[i].bUsed ;i++){
        printf("%d(%d:%d) ",pThreadTbEnt[i].pThread->pid,pThreadTbEnt[i].pThread->priority,pThreadTbEnt[i].pThread->status);
    }
    printf("\n------------------------------\n");
}
void print_pReadyQueue(){
    printf("-------pReadyQueue----------------\n");
    for(int i=0;i<MAX_READYQUEUE_NUM;i++){
        //printf("%d ",i);
        if(pReadyQueueEnt[i].queueCount==0) continue;
        Thread* t = pReadyQueueEnt[i].pHead;
        if(t == NULL) continue;
        for(int j=0; j < pReadyQueueEnt[i].queueCount;j++){
            printf("%d(%d:%d) ",t->pid,t->priority,t->status);
            t = t->phNext;
        }
    }
    printf("\n------------------------------\n");
}

void print_pWaitingQueue(){
    printf("-------pWaitingQueue---------------\n");
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
    printf("\n------------------------------\n");
}
int find_tid(int pid){
    for(int i=0;i<MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].pThread->pid==pid)
            return i;
    }
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
