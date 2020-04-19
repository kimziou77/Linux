#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"

enum{FAILED=-1,SUCCESS=0};
int thread_create(thread_t *thread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg)
{
    //생성된 스레드는 priority가 지정하는 ready queue로 넣는다.
    //ThreadTblEnt에 넣고, 
    thread_t pid = clone(); //thread_t ?? 클론하면 어떻게 되는거지...
    //TODO:  여기 malloc 했어요오오오 deallocate장소 항상확인할것
    Thread* pNewThread = (Thread*)malloc(sizeof(Thread));//TCB 하나 만들어서 clone 한 스레드를 관리한다??d이런 느낌인가
    pNewThread->pid=pid;
    pNewThread->priority=priority;
    //
    kill(pid,SIGSTOP);//클론으로 실행된 스래드를 강제정지시킨다.

    
    //TODO: TCB 할당.초기화;->하는중임 나중에 정확히 다시 채우기
    
    //branch..//실행중인 TCB랑 새로 생성된 TCB의 우선순위를 비교한다.
    if(pCurrentThead->priority < pNewThread->priority){//우선순위는 수가 낮은것을 먼저 실행
        //1.차일드 상태를 레디로 바꾸고
        pNewThread->status=THREAD_STATUS_READY;
        //TODO:레디큐로 insert ReadyQueue 이게 그 저번에 구현했던 FreeList같은개념인가..
    }
    else{
        //생성된 스레드가 우선순위가 더 높다면 컨텍스트 스위칭
        //2.실행중인 스레드를 레디큐로-> 상태(실행->레디)
        __ContextSwitch(pCurrentThead,pid);
        pCurrentThead->status=THREAD_STATUS_READY;
        pNewThread->status=THREAD_STATUS_RUN;
    }
}

int thread_suspend(thread_t tid)
// tid 일시정지시키는 함수 wiating queue의 tail로 들어가게 되는데
// 이미 ready queue 또는 waiting queue 에 들어가 있을 수도 있다.
{
    //자기 자신을 멈추는 동작은 non-precondition


    if(tid>=MAX_THREAD_NUM) return FAILED;
    //해당 pid를 찾아서 처리해준다.
    pThreadTbEnt[tid].pThread->status= THREAD_STATUS_WAIT;
    //TODO: waiting queue의 tail로 보낸다.
    //이미 있으면 어떻게 하지 그냥 Tail로만 옮기면 되나?
    return SUCCESS;
    
    //TODO: 여기서 실패를 했다는건 무슨 뜻이지?
    //이미 웨잇상태라는건가..? 아니면 tid가 잘못된 tid라S는건가?
    
}

int thread_cancel(thread_t tid)//자기자신을 캔슬하는경우는 non-precondition
//해당 스레드를 terminate 시키는 함수
{
    //there is no self kill

    kill(tid,SIGKILL);
    //  TCB를 레디, 웨이팅 큐 에서 뺴낸다.
    //  Deallocate tid's thread TCB
    return SUCCESS;
    
}

int thread_resume(thread_t tid)
//잠든 스레드(waiting Thread) 깨우는 함수, 조건에따라 running / ready 둘 다 갈 수 있다. 
{
    if(tid>=MAX_THREAD_NUM) return FAILED;
    // we must to follow PPT's logic
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

        //TODO: waiting queue 에 있던 TCB를 꺼내서
        //run상태로 바꾸기.
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
    getpid();//이건 clone으로 생성된 thread id 라는데 무슨뜻?
    //TCB의 pid와 비교를 해서 동일한 pid를 가진 entry를 반환
     
    //pid=clone()이값이 아니라
    //ThreadTbEnt의 index값을 반환하도록
    //근데 두개가 같은거 아닌가..?
}
