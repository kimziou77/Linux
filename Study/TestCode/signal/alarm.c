#include <signal.h>
#include <stdio.h>
#include <unistd.h>
void myAlarmHandler(int signal);
int main(int argc, char const *argv[]) {
    if (signal(SIGALRM, myAlarmHandler) == SIG_ERR) {
        perror("signal() error!");
    }
    alarm(5);

    puts("Sleep...");
    sleep(100); // sleep 중에 5초후 alarm시그널을 받고 깨어난다.

    return 0;
}
void myAlarmHandler(int signal) { puts("Hello! I'm wake!"); }
