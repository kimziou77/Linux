#include "Headers.h"

int main(void) {
    thread_t tid;
    int arg;
    Init();
    printf("start!\n");
    thread_create(&tid,NULL,0,AppTask,&arg);
    //TestCase도 스레드로 돌아갈 것임.
    RunScheduler();//while(1);
    printf("RunScheduler complete\n");
    
    //이 while(1)을 main에 하라는거야 Runscheduler에 하라는거야?!
    waitpid(tid,NULL);
}
