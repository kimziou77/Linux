#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    pid_t pid;
    int pipefd[2] = {
        0,
    };

    if (pipe(pipefd) == -1) {
        perror("pipe() error!");
        return -1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork() error!");

    } else if (pid == 0) { // Child
        close(pipefd[0]);  // close unused read end

        dup2(pipefd[1], STDOUT_FILENO);
        execl("/bin/cat", "cat", "/etc/passwd", NULL);

    } else {              // Parent
        close(pipefd[1]); // close unused write end

        wait(&pid);

        dup2(pipefd[0], STDIN_FILENO);
        execl("/usr/bin/cut", "cut", "-d", ":", "-f", "1", NULL);
    }

    return 0;
}
