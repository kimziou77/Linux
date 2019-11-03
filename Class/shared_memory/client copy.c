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
    int x = 0; int y = 0; char op = 0;
    mykey = ftok("mykey", 2);
    shmid = shmget(mykey, MAX_SHM_SIZE, IPC_CREAT);
    shmaddr = shmat(shmid, NULL, 0);

    signal(SIGUSR1, signalHandler);

    puts("Input: [x] [op] [y]");
    while (1) {
        printf("<<< ");
        scanf("%d %c %d", &x, &op, &y );// 입력을 받음
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
        memcpy(&data, shmaddr, sizeof(int));
        printf(">> %d\n", data);
    }
}
