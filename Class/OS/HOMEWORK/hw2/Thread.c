#include "Headers.h"
enum{FAILED=-1,SUCCESS=0};
#define STACK_SIZE 1024*64

int FindEmptyThreadTable()
{
    for(int i=0;i<MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].bUsed==FALSE){
            pThreadTbEnt[i].bUsed=TRUE;
            return i;// thread ID
        }
    }
}
int thread_create(thread_t * thread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg)
{
    char* pStack;
    pStack= malloc(STACK_SIZE);

    int flags= SIGCHLD|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_VM;

    thread_t pid = clone(start_routine,(char*)pStack+STACK_SIZE,flags,arg); 
    kill(pid,SIGSTOP);//클론으로 실행된 스래드를 강제정지시킨다.

    //TODO:  TCB할당 -> deallocate 위치 확인할것
    Thread* pNewThread = (Thread*)malloc(sizeof(Thread));//TCB 하나 만들어서 clone 한 스레드를 관리한다?
    pNewThread->pid=pid;
    pNewThread->priority=priority;
    pNewThread->stackSize=STACK_SIZE;
    
    //pNewTCB->stackAddr=

    *thread = FindEmptyThreadTable();//thread_id 빈 TCB table을 찾아 id 값 할당해줌.
    //Thread  Table에 할당!!
    pThreadTbEnt[*thread].pThread = pNewThread;

    
    printf("새로 생성된 스레드의 우선순위 : %d\n",priority);

    //branch..//실행중인 TCB랑 새로 생성된 TCB의 우선순위를 비교한다.
    if(pCurrentThead == NULL){//현재 실행중인 스레드가 없다면
        pNewThread->status=THREAD_STATUS_RUN;//바로 실행해준다.
        pCurrentThead = pNewThread;
        kill(pCurrentThead->pid,SIGCONT);//중지했던 스레드 다시 실행
    }
    else if(pCurrentThead->priority < pNewThread->priority){//수낮은게 우선순위높음
        pNewThread->status=THREAD_STATUS_READY;
        InsertThreadToReadyQueue(pNewThread);//레디큐에 넣는다.
        printf("thread create 레디큐에 넣는다 (else if)\n");
    }
    else{//생성된 스레드가 우선순위가 더 높다면 컨텍스트 스위칭
        //InsertThreadToReadyQueue(pCurrentThead);//실행중인 스레드를 레디큐로 옮기기
        printf("thread create 생성된 스레드가 우선순위가 더 높음 ->컨텍스트 스위칭 (else)\n");
        pCurrentThead->status = THREAD_STATUS_READY;

        pNewThread->status=THREAD_STATUS_RUN;
        //DeleteThreadFromReadyQueue(pNewThread);//current 레디큐에서 빼기
        
        __ContextSwitch(pCurrentThead->pid,pNewThread->pid);
    }

    return thread;
}

int thread_suspend(thread_t tid)
// tid 일시정지시키는 함수 wiating queue의 tail로 들어가게 되는데
// 이미 ready queue 또는 waiting queue 에 들어가 있을 수도 있다.
{
    printf("현재 thread : %d\n",thread_self());

    //자기 자신을 멈추는 동작은 non-precondition
    if(tid<0 || tid >= MAX_THREAD_NUM) return FAILED;//배열 인덱스초과 -> FAILED

    if(pThreadTbEnt[tid].bUsed==FALSE) return FAILED;//멈추려는 스레드가 존재하지 않으면 FAILED
    
    int status = pThreadTbEnt[tid].pThread->status;
    int pid = pThreadTbEnt[tid].pThread->pid;

    if(status==THREAD_STATUS_RUN){
        printf("SIGSTOP1\n");
        kill(pid,SIGSTOP);
        printf("SIGSTOP2\n");
        __ContextSwitch(pid,pReadyQueueEnt->pTail->pid);//레디큐 에 마지막애로 CS
        //Waiting Queue의 Tail에 넣는다.
    }
    else if(status==THREAD_STATUS_READY){
        //TODO: ready에 있는것도 waiting queue 로 넣어야하는건가 가만히 냅두나?
        // -> waiting queue를 넣는걸로 가정하고 프로그램을 짜보겠음
        int po = pThreadTbEnt[tid].pThread->priority;
        for(int i=0;i<pReadyQueueEnt[po].queueCount;i++){
            
            printf("Thread가 ready 상태이고 waiting queue로 넣어야 할 차례입니다\n");
        }
    }
    else if(status==THREAD_STATUS_WAIT){
        //noting happened
    }
    
    //만약 Ready상태를 가만히 놔두게 되면 밑의 문장을 수정해야됨.
    pThreadTbEnt[tid].pThread->status= THREAD_STATUS_WAIT;
    
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

    return SUCCESS;
}

int thread_resume(thread_t tid)
//잠든 스레드(waiting Thread) 깨우는 함수, 조건에따라 running / ready 둘 다 갈 수 있다. 
{
    if(tid<0 || tid >= MAX_THREAD_NUM) return FAILED;//인덱스초과 -> FAILED

    //깨우는 대상의 priority 에 따라 동작이 달라진다.
    Thread* targetTCB = pThreadTbEnt[tid].pThread;//TCB로부터 우선순위를 받아온다.
    if(pCurrentThead->priority < targetTCB->priority){
        //우선순위 작으면 ready queue로 보낸다.
        targetTCB->status=THREAD_STATUS_READY;
        WaitingQueue_To_ReadyQueue(targetTCB);//waiting -> readyQueue
    }
    else{
        
        pCurrentThead->status=THREAD_STATUS_READY;

        targetTCB->status=THREAD_STATUS_RUN;
        //TODO: waiting queue 에 있던 TCB를 꺼내서 지우기 (run상태로 바꿀꺼니까 waiting queue에는 있을 필요가 없음.)

        pCurrentThead=targetTCB;
        __ContextSwitch(pCurrentThead,targetTCB);//TODO: 이 순서가 맞는지도 확인할것

    }
    return SUCCESS;
}

thread_t thread_self()
{
    int pid= getpid();
    printf("Thread Self : %d \n",pid);

    

    for(int i=0;i<MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].pThread->pid==pid){
            //printf("현재 스레드 아이디는 : %d\n",i);
            return i;// thread ID
        }            
    }
    printf("NONE ID \n");
}

int thread_join(thread_t tid, void ** retval){
    //child의 TCB랑 child의 thread status를 가지고 온다.

    //이때 join을 하기 앞서, child가 먼저 종료되었을 수 있다. 이 경우를 확인하기 위해 Child가 좀비인지를 확인하자
    /*
        if child != 좀비
        {
            1. this.status = waiting 로 셋팅. 왜냐면 child가 종료될 때 까지 기다려야 해서.
            2. TCB를 waiting queue로 보낸다.

            새 스레드를 선택해서 레디큐에서 뺀후에
            상태를 running으로 바꾸고
            
            contextSwitching해준다.

            ---그리고 정지되어있는 상태.--- waiting 
            이제 Parent가 깨어났음 -> child thread가 종료되었음.
            : SIGCHLD시그널이 child스레드로부터 생성되었음.
              Signal handler 구현 밑에서부터 코드가 시작되도록 구현해야한다.

            1. 만약 여기서 현재실행중인  CPU보다 우선순위가 높다면 바로 running으로 간다.
            2. 우선순위가 낮다면 ready 상태로 변경
        }
        
        //Zombie Reaping작업 :child thread 청소

        1. child의 TCB안에 있는 exitCode 를 retVal에 할당해준다.
        2. Child의 TCB를 waiting queue에서 지우고,
        child  TCB를 할당해제한다.
        Free(Child)

    */
   
}
int thread_exit(void * retval){
    
}
void print_pThreadEnt(){
    printf("-------pThreadEnt----------------\n");
    for(int i=0;i<MAX_THREAD_NUM && pThreadTbEnt[i].bUsed ;i++){
        printf("%d ",pThreadTbEnt[i].pThread->pid);
    }
    printf("\n------------------------------\n");
}
void print_pReadyQueue(){
    printf("-------pReadyQueue----------------\n");

    for(int i=0;i<MAX_READYQUEUE_NUM;i++){
        Thread* t = pReadyQueueEnt[i].pHead;
        for(int j=0; j < pReadyQueueEnt[i].queueCount;j++){
            printf("%d ",t->pid);
            t = t->phNext;
        }
    }
    printf("\n------------------------------\n");
}

void print_pWaitingQueue(){
    printf("-------pWaitingQueue---------------\n");
    Thread * t = pWaitingQueueHead;

    for(int i=0;t!=pWaitingQueueTail; t=t->phNext){
        printf("%d ",t->pid);
    }
    printf("\n------------------------------\n");
}
void print_pCurrentThread(){
    printf("-------pCurrentThread---------------\n");
    printf("%d",pCurrentThead->pid);
    printf("\n------------------------------\n");
}
int find_tid(int pid){
    for(int i=0;i<MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].pThread->pid==pid)
            return i;
    }
}
