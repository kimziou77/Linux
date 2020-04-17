#include "Init.h"
#include "Scheduler.h"
#include "Thread.h"
#include <stdio.h>

int main(void) {
    thread_t tid;//thread_id??

    int arg;
    Init();

    thread_create(&tid,NULL,4,AppTask,&arg);
    RunScheduler();
}
