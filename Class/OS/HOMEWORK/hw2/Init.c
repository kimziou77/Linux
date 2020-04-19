#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"

void Init(void)
{
    //Create ready queue and waiting queue
    // Initailize thread scheduler
    for(int i=0;i<MAX_THREAD_NUM;i++){
        pThreadTbEnt[i].bUsed   = FALSE;
        pThreadTbEnt[i].pThread = NULL;
    }
}
