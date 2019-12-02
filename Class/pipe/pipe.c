#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int num = 0;
    pid_t pid;
    int pipefd[2] = {0,};
    srand(time(NULL));
    if(pipe(pipefd)==-1){
        perror("pipe() error");
        return -1;
    }

    pid = fork();
    if(pid==-1){
        perror("fork() error!");
    }else if(pid==0){
      close(pipefd[0]);
      num=rand()%10;
      write(pipefd[1],(int*)&num,sizeof(int));
      close(pipefd[1]);
    }else{
        close(pipefd[1]);
        while(read(pipefd[0],(int*)&num, sizeof(int))<0);
        printf("Num :%d\n",num);
        close(pipefd[0]);
        wait(&pid);
    }
    return 0;
}
