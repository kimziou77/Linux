#ifndef __READY_QUEUE_H__
#define __READY_QUEUE_H__
#include "Thread.h"

//TODO: 임시로 여기다가 선언 넣어줍미당
void * AppTask(void* param);

void InsertThreadToReadyQueue(Thread *pObj, int ObjNum);
void InsertThreadToHead(Thread *pObj, int objNum);
Thread *GetThreadByNum(int objnum);
Thread *GetThreadFromObjFreeList();
BOOL DeleteThreadFromReadyQueue(Thread *pObj);
void WaitingQueue_To_ReadyQueue(Thread *pObj);

#endif
