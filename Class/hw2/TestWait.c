#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
	int val =10;
	pid_t pid =0;
	int status =0;
	
	printf("Hello. My PID is : %d\n",getpid());
	pid = fork();
	if(pid==-1){
		perror("fork() error!");
		exit(-1);
	}
	else if(pid ==0){
		printf("I'm child. My PID is : %d\n",getpid());
		printf("I'm child. My parendt's PID is : %d\n",getppid());
		val+=1;
	}
	else{
		printf("I'm parent. My child's PID is : %d\n", pid);
		wait(&status);
		printf("I'm parent. My child is exited\n");
	}
	printf("My PID is : %d, val is : %d\n",getpid(),val);

    return 0;
}
