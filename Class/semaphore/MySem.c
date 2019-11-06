#include "MySem.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
int creatSem(int key) {
    int semid = semget((key_t)key, SEM_MAX_NUM, IPC_CREAT | PERMS);
    if (semid == -1) {
        perror("creatSem() error!");
        exit(-1);
    }
    return semid;
}
int openSem(int key) {
    int semid = semget((key_t)key, 0, IPC_CREAT);
    return semid;
}
int lockSem(int semid) {
    struct sembuf buf = {0, -1, SEM_UNDO};
    return semop(semid, &buf, SEM_MAX_NUM);
}
int unlockSem(int semid) {
    struct sembuf buf = {0, 1, SEM_UNDO};
    return semop(semid, &buf, SEM_MAX_NUM);
}
int closeSem(int semid) {
    return semctl(semid, 0, IPC_RMID, 0);
}
