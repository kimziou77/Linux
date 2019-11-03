#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    int val = 10;
    pid_t pid = 0;
    int status = 0;
    printf("Hello, My PID is : %d\n", getpid());
    pid = fork();
    if (pid == -1) {
        perror("fork() error!");
        exit(-1);
    } else if (pid == 0) {
        printf("I'm child. My PID is : %d\n", getpid());
        printf("I'm child My parents' PID is %d\n", getppid());
        val += 1;
    } else {
        printf("I'm parent. My child's PID is : %d\n", pid);
        wait(&status);
        printf("I'm parent. My child is exited\n");
    }
    printf("My PID is :%d, val is :5d \n ", getpid(), val);
    return 0;
}

/*
 pid_t pid = 0;
    int status = 0;
    pid = fork();
    if (pid == -1) {
        perror("fork() error!");
        exit(-1);
    } else if (pid == 0) {
        printf("I'm child : %d\n", getpid());
        exit(1);
    } else {
        wait(&status);
        if ((status == 0) || (status > 255)) {
            printf("Child is exited with : %d\n", status >> 8);
        } else {
            printf("Child is terminated with : %d\n", status);
        }
    }

*/
