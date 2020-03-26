/*
 * Student ID :
 * Name
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
    struct shmid_ds buf;
    Calc data;
    int x = 0;
    int y = 0;
    char op = 0;

    mykey = ftok("mykey", 2);
    shmid = shmget(mykey, SHM_MAX_SIZE, IPC_CREAT | PERMS);
    if (shmid == -1) {
        perror("shmget() error!");
        return -1;
    }
    shmaddr = shmat(shmid, NULL, 0);

    signal(SIGINT, signalHandler);
    signal(SIGUSR1, signalHandler);

    while (1) {
        printf("<< ");
        scanf("%d %c %d", &x, &op, &y);
        fflush(stdout);
        fflush(stdin);

        data.x = x;
        data.op = op;
        data.y = y;
        memcpy(shmaddr, &data, sizeof(Calc));
        shmctl(shmid, IPC_STAT, &buf);
        kill(buf.shm_cpid, SIGUSR1);
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
    int result = 0;

    memcpy(&result, shmaddr, sizeof(int));
    printf(">> %d\n", result);
    fflush(stdout);
}
