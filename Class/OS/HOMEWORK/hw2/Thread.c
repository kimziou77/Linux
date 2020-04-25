#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include "ReadyQueue.h"

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
int thread_create(thread_t *thread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg)
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

    thread = FindEmptyThreadTable();//thread_id 빈 TCB table을 찾아 id 값 할당해줌.
    //Thread  Table에 할당!!
    pThreadTbEnt[(int)thread].pThread = pNewThread;

    //branch..//실행중인 TCB랑 새로 생성된 TCB의 우선순위를 비교한다.
    if(pCurrentThead == NULL){//현재 실행중인 스레드가 없다면
        pNewThread->status=THREAD_STATUS_RUN;//바로 실행해준다.
        pCurrentThead = pNewThread;
        kill(pCurrentThead->pid,SIGCONT);//중지했던 스레드 다시 실행
    }
    else if(pCurrentThead->priority < pNewThread->priority){//수낮은게 우선순위높음
        pNewThread->status=THREAD_STATUS_READY;
        InsertThreadToReadyQueue(pNewThread,pNewThread->priority);
    }
    else{//생성된 스레드가 우선순위가 더 높다면 컨텍스트 스위칭
        InsertThreadToReadyQueue(pCurrentThead,pCurrentThead->priority);//실행중인 스레드를 레디큐로 옮기기
        pCurrentThead->status=THREAD_STATUS_READY;

        pNewThread->status=THREAD_STATUS_RUN;
        DeleteThreadFromReadyQueue(pNewThread);//current 레디큐에서 빼기
        
        __ContextSwitch(pCurrentThead->pid,pNewThread->pid);
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
    
    int status = pThreadTbEnt[tid].pThread->status;
    int pid = pThreadTbEnt[tid].pThread->pid;

    if(status==THREAD_STATUS_RUN){
        kill(pid,SIGSTOP);
        //Waiting Queue의 Tail에 넣는다.
    }
    else if(status==THREAD_STATUS_READY){
        //TODO: ready에 있는것도 waiting queue 로 넣어야하는건가 가만히 냅두나?
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
        InsertThreadToReadyQueue(pCurrentThead,pCurrentThead->priority);//현재 실행중인 Threadfmf Ready큐로 보내기

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

    for(int i=0;i<MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].pThread==pid)
            return i;// thread ID
    }
}
