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
    char            data[MAX_MSG_LEN];
    int             size;
    unsigned int    priority; 
    Message*        pNext;
    Message*        pPrev;
} Message;

typedef struct _Qcb {
	int         msgCount;
    Message*    pMsgHead;
    Message*    pMsgTail;
	int         waitThreadCount;
	Thread*     pWaitQHead;
    Thread*     pWaitQTail;
} Qcb;


typedef struct _QcbTblEntry {
    char        name[PMQ_NANE_LEN_MAX];
	int         mode;
	int			openCount;
    Qcb*        pQcb;
    BOOL        bUsed;
} QcbTblEntry;	

QcbTblEntry  qcbTblEntry[MAX_QCB_NUM]; 

pmqd_t pmq_open(const char* name, int flags, mode_t perm, pmq_attr* attr);
int pmq_close(pmqd_t mqd);
int pmq_send(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int msg_prio);
ssize_t pmq_receive(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int* msg_prio);

#endif 
