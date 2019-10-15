#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void mySigHandler(int signum);

int main(int argc, char const *argv[]) {

    if (signal(SIGINT, mySigHandler) == SIG_ERR) {
        perror("signal() error!");
    }
    if (signal(SIGUSR1, mySigHandler) == SIG_ERR) {
        perror("signal() error!");
    }

    printf("Hello My PID is : %d\n", getpid());

    while (1) {
        puts("Pause ...");
        pause();
    }
    return 0;
}
void mySigHandler(int signum) {

    if (signum == SIGINT) {
        printf("Hello! SIGINT.\n");
    } else if (signum == SIGUSR1) {
        printf("Hello! SiGUSR1.\n");
    }
}
