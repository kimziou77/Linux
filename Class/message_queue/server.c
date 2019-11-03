#include "MyMsg.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

void signalHandler(int signum);
int calculate(Calc calc);

key_t mykey=0;
int msqid =0;

int main(int argc, char const *argv[])
{
    MsgCalc msgCalc;
    MsgRslt msgRslt;

    mykey=ftok("mykey",1);
    msqid = msgget(mykey,IPC_CREAT|0644);
    signal(SIGINT,signalHandler);
    while(1){
        puts("Wait ...");
        memset(&msgCalc, 0x00, sizeof(MsgCalc));
        msgrcv(msqid,&msgCalc, MSG_SIZE_CALC,MSG_TYPE_CALC,0);
        printf("Receive: %d %c %d\n",msgCalc.calc.x, msgCalc.calc.op,msgCalc.calc.y);
        memset(&msgRslt,0x00,sizeof(MsgRslt));
        msgRslt.mtype = MSG_TYPE_RSLT;
        msgRslt.result=calculate(msgCalc.calc);
        msgsnd(msqid, &msgRslt, MSG_SIZE_RSLT,0);
        printf("Send : %d\n",msgRslt.result);

        fflush(stdout);
    }
    return 0;
}

void signalHandler(int signum){
    if(signum ==SIGINT){
        msgctl(msqid,IPC_RMID,NULL);
        exit(0);
    }
}
int calculate(Calc calc){
    switch(calc.op){
    case '+':
        return calc.x + calc.y;
    case '-':
        return calc.x - calc.y;
    case '*':
        return calc.x * calc.y;
    case '/':
        return (int)(calc.x/calc.y);
    default:
        return -88;
    }
}
