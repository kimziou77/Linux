#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define PERMS 0666
int main(int argc, char const *argv[])
{
    char * filepath = "./userlist.txt";
    int fd = open(filepath, O_CREAT |O_TRUNC| O_RDWR, PERMS);
    if (fd == -1) {
        perror("open() error");
        return 1;
    }

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
      execl("/bin/cat","cat","/etc/passwd",NULL);
      close(pipefd[1]);
    }
    else{
        close(pipefd[1]);
        wait(&pid);
        dup2(pipefd[0],STDIN_FILENO);

        dup2(fd,STDOUT_FILENO);//표준 출력을 userlist.txt 로  redirection
        execl("/usr/bin/cut","cut","-d",":","-f","1",NULL);
        close(pipefd[0]);

    }
    close(fd);
    return 0;
}
