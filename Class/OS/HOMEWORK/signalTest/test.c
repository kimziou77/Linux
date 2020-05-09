#include "Headers.h"
#include "TestCase1.h"

void* start_routine(void* param){
    while(1){
        sleep(2);
        kill(getppid(),SIGUSR1);
        printf("fffffff");// SIGUSR1 -> ffffff;
        printf("gggggggg");// SIGUSR1 -> ffffff;
        printf("hhhhhhhhh");// SIGUSR1 -> ffffff;
        printf("iiiiiiiiii");// SIGUSR1 -> ffffff;
        printf("jjjjjjjjj");// SIGUSR1 -> ffffff;
        printf("j3");// SIGUSR1 -> ffffff;
        printf("55");// SIGUSR1 -> ffffff;
        printf("66");// SIGUSR1 -> ffffff;
        printf("77");// SIGUSR1 -> ffffff;
        printf("88");// SIGUSR1 -> ffffff;

    }
    
}
void signalHandler(int signum){
        if(signum==SIGALRM){
            printf("awake\n");
        }
        printf("hello I'm SIGUSR1\n");
}
int main(void){

    if(signal(SIGALRM, signalHandler)==SIG_ERR){
        perror("signal() error!");
    }
    alarm(2);
    pause();    
    if(signal(SIGUSR1, signalHandler)==SIG_ERR){
        perror("signal() error!");
    }
    
    // char* pStack;
    // pStack= malloc(STACK_SIZE);
    // int flags= SIGCHLD|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_VM;
    // thread_t pid = clone(start_routine,(char*)pStack+STACK_SIZE,flags,1); 
    // //kill(pid,SIGSTOP);

    while(1);
    return 0;
}
