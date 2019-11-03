#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void mySigHandler(int signum);
int Cnt = 0;
int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Usage : %s [count]", argv[0]);
        exit(0);
    }

    if (signal(SIGUSR1, mySigHandler) == SIG_ERR) {
        perror("signal() error!");
    }
    printf("Hello. My PID is : %d\n", getpid());
    Cnt = atoi(argv[1]);
    while (Cnt > 0) {
        printf("%d ...", Cnt);
        fflush(stdout);
        Cnt -= 1;
        sleep(1);
    }
    puts("");
    return 0;
}
void mySigHandler(int signum) {
    if (signum == SIGUSR1) {
        Cnt = 3;
    }
}
