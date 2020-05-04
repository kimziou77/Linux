#ifndef __READY_QUEUE_H__
#define __READY_QUEUE_H__
#include "Headers.h"

void InsertThreadToReadyQueue(Thread *pObj, int ObjNum);
void InsertThreadToHead(Thread *pObj, int objNum);
Thread *GetThreadByNum(int objnum);
Thread *GetThreadFromWaitingQueue();
BOOL DeleteThreadFromReadyQueue(Thread *pObj);
void WaitingQueue_To_ReadyQueue(Thread *pObj);

#endif
