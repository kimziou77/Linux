#include "hw1.h"

enum Bool { false, true };
void Init() {}

//앞뒤 NULL 설정해주기, FreeList에서
// elementCount 증감시켜주기
int hash_function(int objectNumber, int array_length) {
    // if (array_length == 0) return OBJ_INVALID;
    return objectNumber % array_length;
}

void InsertObjectToTail(Object *pObj, int ObjNum) {
    int idx = hash_function(ObjNum, HASH_TBL_SIZE);
    HashTableEnt *table = &pHashTableEnt[idx];

    if (pHashTableEnt[idx].pHead == NULL) { // ㅁ->null
        pHashTableEnt[idx].pHead = pObj;    // ㅁ->headㅁ
        pHashTableEnt[idx].pTail = pObj;    // ㅁ->headㅁ(tail)
        pHashTableEnt[idx].elmtCount = 1;
        return;
    } else {                                     // ㅁ->ㅁ->ㅁ...
        pHashTableEnt[idx].pTail->phNext = pObj; // tailㅁ  -> newㅁ
        pObj->phPrev = pHashTableEnt[idx].pTail; // tailㅁ <-> newㅁ
        pHashTableEnt[idx].pTail = pObj;         // ㅁ <-> tailㅁ
        pHashTableEnt[idx].elmtCount++;
    }
}

void InsertObjectToHead(Object *pObj, int objNum) {
    int idx = hash_function(objNum, HASH_TBL_SIZE);

    if (pHashTableEnt[idx].pHead == NULL) { // ㅁ->null
        pHashTableEnt[idx].pHead = pObj;    // ㅁ->headㅁ
        pHashTableEnt[idx].pTail = pObj;    // ㅁ->headㅁ(tail)
        pHashTableEnt[idx].elmtCount = 1;
        return;
    } else {                                     // ㅁ->ㅁ->ㅁ...
        pObj->phNext = pHashTableEnt[idx].pHead; // new  ㅁ  -> ㅁ head
        pHashTableEnt[idx].pHead->phPrev = pObj; // new  ㅁ <-> ㅁ head
        pHashTableEnt[idx].pHead = pObj;         // head ㅁ <-> ㅁ
        pHashTableEnt[idx].elmtCount++;
    }
}

Object *GetObjectByNum(int objnum) {
    int idx = hash_function(objnum, HASH_TBL_SIZE);
    Object *cursor = pHashTableEnt[idx].pHead;
    while (cursor) {
        if (cursor->objnum == objnum) {
            return cursor;
        }
        cursor = cursor->phNext;
    }
    return NULL;
}

Object *GetObjectFromObjFreeList() {
    if (pFreeListTail == NULL)
        return NULL;
    Object *target = pFreeListTail;

    Object *prev = target->phPrev;
    prev->phNext = pFreeListHead;
    pFreeListHead->phPrev = prev;
    pFreeListTail = prev;

    target->phNext = NULL;
    target->phPrev = NULL;
    return target;
}

BOOL DeleteObject(Object *pObj) {
    int idx = hash_function(pObj->objnum, HASH_TBL_SIZE);

    // no Object -> false;
    if (GetObjectByNum(pObj->objnum) == NULL)
        return false;

    if (pHashTableEnt[idx].pHead == pObj) { //ㅁhead(obj) <-> ㅁ <-> ㅁ....
        pHashTableEnt[idx].pHead = pObj->phNext; //ㅁ(obj) <-> ㅁhead <-> ㅁ...
        pHashTableEnt[idx].pHead->phPrev = NULL; //ㅁ(obj)  -> ㅁhead <-> ㅁ...
    } else { // ㅁhead <-> ㅁ(obj) <-> ㅁ...
        Object *target = GetObjectByNum(pObj->objnum);
        Object *prev = target->phPrev; // ㅁprev <-> ㅁ <-> ㅁnext
        Object *next = target->phNext;
        prev->phNext = next; // ㅁprev  -> ㅁnext
        next->phPrev = prev; // ㅁprev <-> ㅁnext
    }
    pObj->phNext = NULL;
    pObj->phPrev = NULL;
    pHashTableEnt[idx].elmtCount--;
    return true;
}

void InsertObjectIntoObjFreeList(Object *pObj) {
    // if needed, the number of the object is set to OBJ_INVALID
    // head->prev==NULL?
    // into the head
    if (pFreeListHead == NULL) {
        pFreeListHead = pObj;
        pFreeListTail = pObj;
    } else {

        pFreeListTail->phNext = pObj;
        pFreeListHead->phPrev = pObj;

        pObj->phPrev = pFreeListTail;
        pObj->phNext = pFreeListHead; // head <-> obj <-> tail

        pFreeListHead = pObj;
    }
}
