#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#define TRUE 1
#define FALSE 0

void mySigHandler(int signum);
int cnt = 100;
int main(int argc, char const *argv[]) {

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL); // USR1 블록

    signal(SIGUSR1, mySigHandler);
    signal(SIGUSR2, mySigHandler);
    int key = TRUE;
    printf("my PID is : %d\n", getpid());
    while (cnt > 0) {
        printf("%d\n", cnt);
        fflush(stdout);
        cnt -= 1;
        sleep(1); // 50초간 USR1은 동작하지 않음. 2는 가능 5
        if (cnt < 50 && key) {
            sigprocmask(SIG_UNBLOCK, &set, NULL); // USR1 Block 해제
            sigaddset(&set, SIGUSR2);             // set 에 USR2 등록
            sigprocmask(SIG_BLOCK, &set, NULL);
            key = FALSE;
        }
        sigprocmask(SIG_UNBLOCK, &set, NULL);
    }
    return 0;
}
void mySigHandler(int signum) {
    if (signum == SIGUSR1) {
        puts("Hello");
    } else if (signum == SIGUSR2) {
        puts("Bonjours!");
        cnt = 50;
    }
}
