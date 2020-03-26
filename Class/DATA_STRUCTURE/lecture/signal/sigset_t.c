#include <signal.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    printf("SIGUSR1 : %d\n", sigismember(&set, SIGUSR1));
    printf("SIGUSR2 : %d\n", sigismember(&set, SIGUSR2));

    return 0;
}
