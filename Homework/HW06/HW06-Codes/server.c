/*
 * Student ID :2018203062
 * Name : 김수빈
 */
#include "MyShm.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_SHM_SIZE 512

void signalHandler(int signum);
void myfunc(void);
int calculate(Calc calc);
key_t mykey = 0;
int shmid = 0;
int *shmaddr = NULL;

int main(int argc, char const *argv[])
{
    mykey = ftok("mykey", 2);
    shmid = shmget(mykey, MAX_SHM_SIZE, IPC_CREAT | 0644);
    shmaddr = shmat(shmid, NULL, 0);
    signal(SIGINT, signalHandler);
    signal(SIGUSR1, signalHandler);
    while (1) {
        puts("Wait ...");
        pause();
    }

    return 0;
}

void signalHandler(int signum) {
    struct shmid_ds buf;
    if (signum == SIGINT) {
        shmdt(shmaddr);
        shmctl(shmid, IPC_RMID, NULL);
        exit(0);
    } else if (signum == SIGUSR1) {
        myfunc();
    }
}
void myfunc(void) {
    struct shmid_ds buf;
    Calc calc;
    int data=0;
    memcpy(&calc, shmaddr, sizeof(Calc));
    printf("Receive: %d %c %d\n", calc.x,calc.op,calc.y);
    data = calculate(calc); // 계산결과를 data 에 저장


    memcpy(shmaddr, &data, sizeof(int));

    printf("Send: %d\n", data);
    shmctl(shmid, IPC_STAT, &buf);
    kill(buf.shm_lpid, SIGUSR1);
}
int calculate(Calc calc){
    switch(calc.op){
    case '+':
        return calc.x+calc.y;
    case '-':
        return calc.x-calc.y;
    case '*':
        return calc.x*calc.y;
    case '/':
        return calc.x/calc.y;
    default :
        return 0;
    }
}
