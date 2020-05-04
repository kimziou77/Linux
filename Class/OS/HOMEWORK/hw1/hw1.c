#include "hw1.h"

enum { FALSE, TRUE };

int hash_function(int objectNumber, int array_length) {//v find tid
	return objectNumber % array_length;
}
void Init() {//v Init
	pFreeListHead = NULL;
	pFreeListTail = NULL;
	for (int i = 0; i < HASH_TBL_SIZE; i++) {
		pHashTableEnt[i].pHead = NULL;
		pHashTableEnt[i].pTail = NULL;
	}
}

void InsertObjectToTail(Object* pObj, int ObjNum) {//v insert Trhread To Ready Queue

	int idx = hash_function(ObjNum, HASH_TBL_SIZE);
	pObj->objnum = ObjNum;

	if (pHashTableEnt[idx].pHead == NULL) {	// ㅁ->null
		pHashTableEnt[idx].pHead = pObj;	// ㅁ->headㅁ
		pHashTableEnt[idx].pTail = pObj;	// ㅁ->headㅁ(tail)

		pHashTableEnt[idx].elmtCount = 1;
	}
	else {// ㅁ->ㅁ->ㅁ...
		pHashTableEnt[idx].pTail->phNext = pObj;	// tailㅁ  -> newㅁ
		pObj->phPrev = pHashTableEnt[idx].pTail;	// tailㅁ <-> newㅁ
		pHashTableEnt[idx].pTail = pObj;			// ㅁ <-> tailㅁ

		pHashTableEnt[idx].elmtCount++;
	}
}

void InsertObjectToHead(Object* pObj, int objNum) { //헤드로 넣을 필요 xxxx
	int idx = hash_function(objNum, HASH_TBL_SIZE);
	pObj->objnum = objNum;
	if (pHashTableEnt[idx].pHead == NULL) {	// ㅁ->null
		pHashTableEnt[idx].pHead = pObj;	// ㅁ->headㅁ
		pHashTableEnt[idx].pTail = pObj;	// ㅁ->headㅁ(tail)
		pHashTableEnt[idx].elmtCount = 1;
	}
	else {// ㅁ->ㅁ->ㅁ...
		pObj->phNext = pHashTableEnt[idx].pHead;	//new  ㅁ  -> ㅁ head
		pHashTableEnt[idx].pHead->phPrev = pObj;	//new  ㅁ <-> ㅁ head
		pHashTableEnt[idx].pHead = pObj;			//head ㅁ <-> ㅁ
		pHashTableEnt[idx].elmtCount++;
	}
}

Object* GetObjectByNum(int objNum) {// v GetThreadbyPid
	int idx = hash_function(objNum, HASH_TBL_SIZE);
	Object* cursor = pHashTableEnt[idx].pHead;

	while (cursor) {
		if (cursor->objnum == objNum) {
			return cursor;
		}
		cursor = cursor->phNext;
	}
	return NULL;
}

Object* GetObjectFromObjFreeList() {//v GetThreadFromWaitingQueue
	if (pFreeListTail == NULL) return NULL;
	Object* target = pFreeListTail;

	//prev가 nullptr이었음.
	Object* prev = target->phPrev;

	if (prev == NULL) {
		pFreeListHead = NULL;
		pFreeListTail = NULL;
	}
	else {
		prev->phNext = NULL;  //ㅁ(prev)->null, ㅁ(target,tail)
		//pFreeListHead->phPrev = prev;
		pFreeListTail = prev;  //ㅁ(prev,tail), ㅁ(target)

		target->phNext = NULL;
		target->phPrev = NULL;
	}
	return target;
}

BOOL DeleteObject(Object* pObj) {//Delete Thread From ReadyQueue

	int idx = hash_function(pObj->objnum, HASH_TBL_SIZE);
	Object* obj = GetObjectByNum(pObj->objnum);

	//no Object -> false;
	if (obj == NULL) return FALSE;

	if (pHashTableEnt[idx].pHead == pObj){ // 1. 헤드쪽에 위치
		if (pHashTableEnt[idx].elmtCount == 1) { // 헤드쪽에 삭제하려는것 하나만 존재
			pHashTableEnt[idx].pHead = NULL;
		}
		else{
			pHashTableEnt[idx].pHead = pObj->phNext;	//ㅁ(obj) <-> ㅁhead <-> ㅁ...
			pHashTableEnt[idx].pHead->phPrev = NULL;	//ㅁ(obj)  -> ㅁhead <-> ㅁ...
		}
	}
	else if (pHashTableEnt[idx].pTail == pObj) { // 2. 테일쪽에 위치
		if (pHashTableEnt[idx].elmtCount == 1) { // 테일쪽에 삭제하려는것 하나만 존재
			pHashTableEnt[idx].pTail = NULL;
		}
		else {
			pHashTableEnt[idx].pTail = pObj->phPrev;
			pHashTableEnt[idx].pTail->phNext = NULL;
		}
	}
	else {									// 3. 중앙에 위치
		Object* target = obj;
		Object* prev = target->phPrev;					// ㅁprev <-> ㅁ <-> ㅁnext
		Object* next = target->phNext;
		prev->phNext = next;							// ㅁprev  -> ㅁnext
		next->phPrev = prev;							// ㅁprev <-> ㅁnext
	}


	pObj->phNext = NULL;
	pObj->phPrev = NULL;
	pHashTableEnt[idx].elmtCount--;
	return TRUE;
}

void InsertObjectIntoObjFreeList(Object* pObj) {
	pObj->phNext = NULL;	pObj->phPrev = NULL;

	if (pFreeListHead == NULL) {//비어있다면 넣는다.
		pFreeListHead = pObj;
		pFreeListTail = pObj;
	}
	else {
		pFreeListHead->phPrev = pObj; // null<- ㅁ(obj) <- ㅁ(head)

		pObj->phNext = pFreeListHead;  //ㅁ(obj)<-> ㅁ(head)

		pFreeListHead = pObj; // ㅁ(obj,head)<->ㅁ
	}
}
