/*
 * Student ID :
 * Name :
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

void signalHandler(int signum);
void myfunc(void);

key_t mykey = 0;
int shmid = 0;
int *shmaddr = NULL;

int main(int argc, char const *argv[]) {
    mykey = ftok("mykey", 2);
    shmid = shmget(mykey, SHM_MAX_SIZE, IPC_CREAT | IPC_EXCL | PERMS);
    if (shmid == -1) {
        perror("shmget() error!");
        return -1;
    }
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
    Calc data;
    int result = 0;

    memcpy(&data, shmaddr, sizeof(Calc));
    printf("Receive: %d %c %d\n", data.x, data.op, data.y);

    switch (data.op) {
    case '+':
        result = data.x + data.y;
        break;
    case '-':
        result = data.x - data.y;
        break;
    case '*':
        result = data.x * data.y;
        break;
    case '/':
        result = (int)(data.x / data.y);
        break;
    default:
        result = 0;
        break;
    };

    memcpy(shmaddr, &result, sizeof(int));
    printf("Send: %d\n", result);
    shmctl(shmid, IPC_STAT, &buf);
    kill(buf.shm_lpid, SIGUSR1);
}
