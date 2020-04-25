#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"

//TODO: 여기다 정의해도 되나?



BOOL Is_Ready_Queue_Empty(){
    for(int i=0;i<MAX_READYQUEUE_NUM;i++){
        if(pReadyQueueEnt[i].queueCount != 0) return FALSE;
    }
    return TRUE;    
}
int RunScheduler( void )
{
    if(Is_Ready_Queue_Empty()){ //레디큐에 무언가 존재한다면??
        /*
        Priority based Round Robin
        running스레드를 readyQueue의 tail로 넣기.
        readyQueue에서 새로운 스레드TCB를 제거한다.
        그리고 contextSwitching 한다.
        __ContextSwitch(curpid, -- );
        */
    }

    alarm(TIMESLICE); // keep doing RunScheduler
    //주기적으로(?) 시그널을 발생시켜준다.

    //SIGARLM 이라는 시그널이 발생함.
    //시그널 핸들러를 만들어주어야됨
    //시그널 핸들러 안에서 무엇을 해야한다면
    //Priority based Round Robin을 해줘야함.
    //알람이 울리면 RunScheduler 실행해주기?
}

//systemcall로 생성을 했으므로
//systemcall을 이용해 contextSwitching을 할 것.
void __ContextSwitch(int curpid, int newpid)
{//여기 인자로 들어오는거는 TCB의 thread id인가 아니면 그냥 pid 인가.....
    kill(curpid, SIGSTOP);
    kill(newpid, SIGCONT);
    //pCurrentThead; 여기 TCB를 담아야 하는데 그러면 인자는 thread id 인가..?
}
