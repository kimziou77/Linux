#define _GNU_SOURCE
#include <sched.h>

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>


#define STACK_SIZE 1024*64

int foo(void* a){
    //thread code
}
int threadFunc(void* a){

}
int main(void){
    void * stack;
    stack= malloc(STACK_SIZE);
    
    pid_t pid;
    if(stack==0){
        perror("*malloc : could not allocate stack");
        exit(1);
    }
    
    pid= clone(&foo,(char*)stack+STACK_SIZE,CLONE_VM|CLONE_SIGHAND|CLONE_FS|CLONE_FILES,0);//프로세스가 아니라 스레드가 생성이 된다.

    int arg=100;
    int flags= SIGCHLD|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_VM;
    
    char *pStack;
    pStack= malloc(STACK_SIZE);
    pid=clone(threadFunc,(char*)pStack+STACK_SIZE, flags, &arg);
    waitpid(pid,NULL);

}
