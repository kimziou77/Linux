#ifndef __READY_QUEUE_H__
#define __READY_QUEUE_H__
#include "Headers.h"

void InsertThreadToReadyQueue(Thread *pThread);
void InsertThreadToWaitingQueue(Thread *pThread);

Thread *GetThreadByNum(int pid);

Thread *GetThreadFromWaitingQueue(int pid);
Thread * GetThreadFromReadyQueue();

BOOL DeleteThreadFromReadyQueue(Thread *pThread);
void WaitingQueue_To_ReadyQueue(Thread *pThread);
void DeleteThreadFromWaitingQueue(Thread *pThread);
BOOL IsReadyQueueEmpty();
#endif
