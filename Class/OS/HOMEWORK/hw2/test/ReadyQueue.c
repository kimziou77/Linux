#include <sched.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "Thread.h"

#ifndef __HW1_H__
#define __HW1_H__

#include <stdio.h>

#define HASH_TBL_SIZE (8)
#define MAX_OBJLIST_NUM (3)
#define OBJ_INVALID (-1)

typedef struct Object Object;
typedef int BOOL;

struct Object {
    //얘는 TCB로 변환될것
};

typedef struct __ReadyQueueEnt {
    int queueCount;
    Object *pHead;
    Object *pTail;
} __ReadyQueueEnt;

//HashTableEnt pHashTableEnt[HASH_TBL_SIZE];

Thread* *pWaitingQueueHead;
Thread* *pWaitingQueueTail;

void Init();
void InsertObjectToTail(Object *pObj, int ObjNum);
void InsertObjectToHead(Object *pObj, int objNum);
Object *GetObjectByNum(int objnum);
Object *GetObjectFromObjFreeList();
BOOL DeleteObject(Object *pObj);
void InsertObjectIntoObjFreeList(Object *pObj);

#endif
