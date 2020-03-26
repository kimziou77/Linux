#include "MyShm.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
int creatShm(int key) {
    int shmid = shmget((key_t)key, SHM_MAX_SIZE, IPC_CREAT | IPC_EXCL | PERMS);
    if (shmid == -1) {
        perror("creatShm() error!");
        exit(-1);
    }
    return shmid;
}
int openShm(int key) {
    int shmid = shmget((key_t)key, SHM_MAX_SIZE, IPC_CREAT);
    return shmid;
}
int closeShm(int shmid) { return shmctl(shmid, IPC_RMID, 0); }
