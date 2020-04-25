#ifndef __READY_QUEUE_H__
#define __READY_QUEUE_H__
#include <stdio.h>
#include "Thread.h"

void InsertThreadToTail(Thread *pObj, int ObjNum);
void InsertThreadToHead(Thread *pObj, int objNum);
Thread *GetThreadByNum(int objnum);
Thread *GetThreadFromObjFreeList();
BOOL DeleteThread(Thread *pObj);
void InsertThread_Into_ReadyQueue(Thread *pObj);

#endif
