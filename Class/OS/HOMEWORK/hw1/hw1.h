<<<<<<< HEAD
#ifndef __HW1_H__
#define __HW1_H__

#include <stdio.h>

#define HASH_TBL_SIZE    (8)
#define MAX_OBJLIST_NUM  (3)
#define OBJ_INVALID  (-1)

typedef struct Object Object;
typedef int BOOL;

struct Object
{
    int           objnum;
    Object*       phPrev;
    Object*       phNext; 
};

typedef struct __HashTableEnt
{
    int elmtCount; 
    Object*  pHead;
    Object*  pTail;
} HashTableEnt;

HashTableEnt pHashTableEnt[HASH_TBL_SIZE];


Object*  pFreeListHead;
Object*  pFreeListTail;


void Init();
void InsertObjectToTail(Object* pObj, int ObjNum);
void InsertObjectToHead(Object* pObj, int objNum);
Object* GetObjectByNum(int objnum);
Object* GetObjectFromObjFreeList();
BOOL DeleteObject(Object* pObj);
void InsertObjectIntoObjFreeList(Object* pObj);

#endif
=======
#ifndef __HW1_H__
#define __HW1_H__

#include <stdio.h>

#define HASH_TBL_SIZE    (8)
#define MAX_OBJLIST_NUM  (3)
#define OBJ_INVALID  (-1)

typedef struct Object Object;
typedef int BOOL;

struct Object
{
    int           objnum;
    Object*       phPrev;
    Object*       phNext; 
};

typedef struct __HashTableEnt
{
    int elmtCount; 
    Object*  pHead;
    Object*  pTail;
} HashTableEnt;

HashTableEnt pHashTableEnt[HASH_TBL_SIZE];


Object*  pFreeListHead;
Object*  pFreeListTail;


void Init();
void InsertObjectToTail(Object* pObj, int ObjNum);
void InsertObjectToHead(Object* pObj, int objNum);
Object* GetObjectByNum(int objnum);
Object* GetObjectFromObjFreeList();
BOOL DeleteObject(Object* pObj);
void InsertObjectIntoObjFreeList(Object* pObj);

#endif
>>>>>>> 1d05ca882ea2d3d0f4ec427b24fb1db71707ee7a
