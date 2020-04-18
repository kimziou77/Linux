#include "Init.h"
#include "Scheduler.h"
#include "Thread.h"
#include <stdio.h>

int main(void) {
    thread_t tid;//thread_id??

    int arg;
    Init();

    thread_create(&tid,NULL,4,AppTask,&arg);
    //TestCase도 스레드로 돌아갈 것임.
    RunScheduler();//while(1);
    //TODO: 여기서 부터 분석해보기

}
