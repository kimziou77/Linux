#include "Headers.h"

int main(void) {
    thread_t tid;
    int arg;
    Init();
    thread_create(&tid,NULL,4,AppTask,&arg);
    //TestCase도 스레드로 돌아갈 것임.
    RunScheduler();//while(1);
    //이 while(1)을 main에 하라는거야 Runscheduler에 하라는거야?!
}
