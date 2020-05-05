#include "Headers.h"

int main(void) {
    printf("main : %d",getpid());
    thread_t tid;
    int arg;
    Init();
    thread_create(&tid,NULL,4,AppTask,&arg);
    RunScheduler();//while(1);
    
    while(1){}
}
