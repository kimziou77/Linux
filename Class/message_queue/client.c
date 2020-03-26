#include "MyMsg.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    key_t mykey=ftok("mykey",1);
    int msqid = msgget(mykey,IPC_CREAT);

    int x=0;
    int y=0;
    char op=0;
    MsgCalc msgCalc;
    MsgRslt msgRslt;

    puts("intput : [x] [op] [y]");
    while(1){
        printf("<<<");
        scanf("%d %c %d",&x,&op,&y);
        fflush(stdout);
        fflush(stdin);
        memset(&msgCalc,0x00,sizeof(MsgCalc));
        msgCalc.mtype = MSG_TYPE_CALC;
        msgCalc.calc.x=x;
        msgCalc.calc.y=y;
        msgCalc.calc.op=op;
        msgsnd(msqid, &msgCalc,MSG_SIZE_CALC,0);

        memset(&msgRslt,0x00,sizeof(MsgRslt));
        msgrcv(msqid, &msgRslt,MSG_SIZE_RSLT,MSG_TYPE_RSLT,0);
        printf(">>> %d\n",msgRslt.result);
    }
    return 0;
}
