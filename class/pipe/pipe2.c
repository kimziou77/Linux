#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t pid;
    int pipefd[2] = {0,};

    if(pipe(pipefd)==-1){
        perror("pipe() error");
        return -1;
    }
    pid = fork();

    if(pid==-1){
        perror("fork() error!");
    }
    else if(pid==0){
      close(pipefd[0]);

      dup2(pipefd[1],STDOUT_FILENO);
      execl("/bin/ls","ls","-a","-l",NULL);
      close(pipefd[1]);
    }
    else{
        close(pipefd[1]);
        wait(&pid);

        dup2(pipefd[0],STDIN_FILENO);
        execl("/usr/bin/sort","sort","-r",NULL);
        close(pipefd[0]);
    }
    return 0;
}
