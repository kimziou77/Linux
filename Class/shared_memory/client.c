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

key_t mykey = 0;
int shmid = 0;
int *shmaddr = NULL;

int main(int argc, char const *argv[]) {
    struct shmid_ds buf;
    int data = 0;
    mykey = ftok("mykey", 2);
    shmid = shmget(mykey, MAX_SHM_SIZE, IPC_CREAT);

    shmaddr = shmat(shmid, NULL, 0);
    signal(SIGINT, signalHandler);
    signal(SIGUSR1, signalHandler);

    while (1) {
        puts("Wait...");
        pause();

        fflush(stdout);
        fflush(stdin);
        memcpy(shmaddr, &data, sizeof(int));
        shmctl(shmid, IPC_STAT, &buf);
        kill(buf.shm_cpid, SIGUSR1);
        pause();
    }

return 0;
}

void signalHandler(int signum) {
    int data = 0;
    if (signum == SIGUSR1) {
        myfunc();
    }else if(signum==SIGINT){
        shmdt(shmaddr);
        shmctl(shmid,IPC_RMID,NULL);
        exit(0);
    }
}
void myfunc(void){
    struct shmid_ds buf;
    Calc calc;

}
