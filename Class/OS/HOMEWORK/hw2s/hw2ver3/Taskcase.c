#include <stdio.h>
void * foo1(void* arg){

    while(1);
}
void * foo2(void* arg){

    while(1);
}
void * foo3(void* arg){

    while(1);
}

void Tastcase(void){
    int tid1,tid2,tid3;
    int arg1,arg2,arg3;

    thread_create(&tid1,NULL,0,foo1,&arg1);
    thread_create(&tid2,NULL,1,foo2,&arg2);
    thread_create(&tid3,NULL,2,foo3,&arg3);

    thread_suspend(tid1);
    thread_suspend(tid2);

    thread_resume(tid1);
    while(1);
}
