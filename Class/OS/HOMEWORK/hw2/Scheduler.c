#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"

int RunScheduler( void )
{

    //Ready queue is empty?
    /*
        Priority based Round Robin

        __ContextSwitch
    */

    alarm(TIMESLICE); // keep doing RunScheduler

}


void __ContextSwitch(int curpid, int newpid)
{
    // pCurrentThead
}
