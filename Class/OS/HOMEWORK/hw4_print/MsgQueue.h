#ifndef __MQ_H__
#define __MQ_H__

#include "Thread.h"
#include "Scheduler.h"

#define MAX_MSG_LEN         (1024)
#define MAX_QCB_NUM         (64)
#define PMQ_NANE_LEN_MAX    (32)

typedef struct _Message Message;
typedef int pmqd_t;
typedef int BOOL;

typedef struct _pmq_attr{
    int maxMsgNum;
    int msgLength;
}pmq_attr;

typedef struct _Message {
    char            data[MAX_MSG_LEN];//메세지의 최대 길이
    int             size;//현재 저장되어있는 메시지의 길이
    unsigned int    priority; //메시지 우선순위
    Message*        pNext;
    Message*        pPrev;
} Message;

typedef struct _Qcb {//메세지 큐 하나 생성할 때 마다 하나씩 생성
	int         msgCount;//메세지 개수
    Message*    pMsgHead;//메세지 저장하는 메시지 리스트 메시지큐(x)
    Message*    pMsgTail;//prio순으로 메시지를 정렬하여 저장하고 있음.
	int         waitThreadCount;//웨이팅 개수
	Thread*     pWaitQHead;//스레드 웨이팅 큐
    Thread*     pWaitQTail;//빈 메시지 큐에서 메시지를 받고자 하는 스레드를 대기 시키기 위해 사용됨
} Qcb;


typedef struct _QcbTblEntry {//qcb를 관리하는 테이블임. thread table과도 같다.
    char        name[PMQ_NANE_LEN_MAX];
	int         mode;
    int         openCount;
    Qcb*        pQcb;
    BOOL        bUsed;
} QcbTblEntry;	

QcbTblEntry  qcbTblEntry[MAX_QCB_NUM]; 

pmqd_t pmq_open(const char* name, int flags, mode_t perm, pmq_attr* attr);
int pmq_close(pmqd_t mqd);
int pmq_send(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int msg_prio);
ssize_t pmq_receive(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int* msg_prio);

#endif 
