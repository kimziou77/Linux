#include "Headers.h"

BOOL Is_Ready_Queue_Empty(){
    for(int i=0;i<MAX_READYQUEUE_NUM;i++){
        if(pReadyQueueEnt[i].queueCount != 0) return FALSE;
    }
    return TRUE;    
}

int RunScheduler( void )
{
    alarm(TIMESLICE); // keep doing 
    while(1){
        //if(Is_Ready_Queue_Empty()){ //레디큐에 무언가 존재한다면??
        /*
        Priority based Round Robin
        running스레드를 readyQueue의 tail로 넣기.
        readyQueue에서 새로운 스레드TCB를 제거한다.
        그리고 contextSwitching 한다.
        __ContextSwitch(curpid, -- );
        */
    }
        
    //이걸 해주고 종료되면 알람이 오나?
    //여기서 멈춰놔야 하지 않을까?

    //주기적으로(?) 시그널을 발생시켜준다.
}

//systemcall로 생성을 했으므로
//systemcall을 이용해 contextSwitching을 할 것.

void    __ContextSwitch(int curpid, int newpid){
//여기 인자로 들어오는거는 TCB의 thread id인가 아니면 그냥 pid 인가.....
    //pCurrentThead = pThreadTbEnt[];
    kill(curpid, SIGSTOP);
    kill(newpid, SIGCONT);
    //pCurrentThead; 여기 TCB를 담아야 하는데 그러면 인자는 thread id 인가..?
    return;
}
