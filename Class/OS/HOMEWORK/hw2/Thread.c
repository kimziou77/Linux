#define _GNU_SOURCE
#include <sched.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"

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
    Thread* pNewTCB = (Thread*)malloc(sizeof(Thread));//TCB 하나 만들어서 clone 한 스레드를 관리한다?
    pNewTCB->pid=pid;
    pNewTCB->priority=priority;//가 지정하는 ready queue에 넣는다.    
    pNewTCB->stackSize=STACK_SIZE;
    //pNewTCB->stackAddr=

    thread = FindEmptyThreadTable();//thread_id 빈 TCB table을 찾아 id 값 할당해줌.
    pThreadTbEnt[(int)thread].pThread = pNewTCB;

    //branch..//실행중인 TCB랑 새로 생성된 TCB의 우선순위를 비교한다.
    if(pCurrentThead->priority < pNewTCB->priority){//우선순위는 수가 낮은것을 먼저 실행

        pNewTCB->status=THREAD_STATUS_READY;
        //new TCB 를 레디큐로 옮기기
    }
    else{//생성된 스레드가 우선순위가 더 높다면 컨텍스트 스위칭

        //실행중인 스레드를 레디큐로 옮기기
        pCurrentThead->status=THREAD_STATUS_READY;

        pNewTCB->status=THREAD_STATUS_RUN;
        //current 레디큐에서 빼기
        pCurrentThead=pNewTCB;

        __ContextSwitch(pCurrentThead->pid,pNewTCB->pid);
    }

    return thread;
}

int thread_suspend(thread_t tid)
// tid 일시정지시키는 함수 wiating queue의 tail로 들어가게 되는데
// 이미 ready queue 또는 waiting queue 에 들어가 있을 수도 있다.
{
    //자기 자신을 멈추는 동작은 non-precondition
    if(tid<0 || tid >= MAX_THREAD_NUM) return FAILED;
    //해당 pid를 찾아서 처리해준다.
    if(pThreadTbEnt[tid].bUsed==FALSE) return FAILED;

    //int status = pThreadTbEnt[tid].pThread->status;
    //if(status==THREAD_STATUS_WAIT|| status==THREAD_STATUS_READY) 

    //waiting queue로 넣는다.

    pThreadTbEnt[tid].pThread->status= THREAD_STATUS_WAIT;
    
    return SUCCESS;
}

int thread_cancel(thread_t tid)//자기자신을 캔슬하는경우는 non-precondition
//해당 스레드를 terminate 시키는 함수 ,there is no self kill
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
    if(tid>=MAX_THREAD_NUM) return FAILED;
    //깨우는 대상의 priority 에 따라 동작이 달라진다.

    Thread* targetTCB = pThreadTbEnt[tid].pThread;//TCB로부터 우선순위를 받아온다.
    if(pCurrentThead->priority < targetTCB->priority){ //현재 실행중인 스레드가 더 우선순위가 높다면(낮은수 우선)
        //그냥 ready queue로 보낸다.
        targetTCB->status=THREAD_STATUS_READY;
        //TODO: targetTCB를 ready_queue 로 보내기
        
    }
    else{
        //TODO:현재 실행중인 Threadfmf Ready큐로 보내기
        pCurrentThead->status=THREAD_STATUS_READY;



        //TODO: waiting queue 에 있던 TCB를 꺼내서 지우기 (run상태로 바꿀꺼니까 waiting queue에는 있을 필요가 없음.)

        targetTCB->status=THREAD_STATUS_RUN;
        __ContextSwitch(pCurrentThead,targetTCB);//TODO: 이 순서가 맞는지도 확인할것

        pCurrentThead=targetTCB;//TODO: 이렇게 바로 바꿔도 되는지 확인할것        
        //num 변수도 확인할것
    }

    //if success return 0;
    //else (failed) return -1;
}

thread_t thread_self()
{
    int pid= getpid();

    for(int i=0;i<MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].pThread==pid)
            return i;// thread ID
    }
}
