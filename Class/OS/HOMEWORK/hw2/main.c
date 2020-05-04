#include "Headers.h"

int main(void) {
    thread_t tid;
    int arg;
    Init();
    thread_create(&tid,NULL,4,AppTask,&arg);
    RunScheduler();//while(1);
    printf("RunScheduler complete\n");
    
    waitpid(tid,NULL);
}
