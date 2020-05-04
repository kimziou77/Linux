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

	if (pHashTableEnt[idx].pHead == NULL) {	// ��->null
		pHashTableEnt[idx].pHead = pObj;	// ��->head��
		pHashTableEnt[idx].pTail = pObj;	// ��->head��(tail)

		pHashTableEnt[idx].elmtCount = 1;
	}
	else {// ��->��->��...
		pHashTableEnt[idx].pTail->phNext = pObj;	// tail��  -> new��
		pObj->phPrev = pHashTableEnt[idx].pTail;	// tail�� <-> new��
		pHashTableEnt[idx].pTail = pObj;			// �� <-> tail��

		pHashTableEnt[idx].elmtCount++;
	}
}

void InsertObjectToHead(Object* pObj, int objNum) { //���� ���� �ʿ� xxxx
	int idx = hash_function(objNum, HASH_TBL_SIZE);
	pObj->objnum = objNum;
	if (pHashTableEnt[idx].pHead == NULL) {	// ��->null
		pHashTableEnt[idx].pHead = pObj;	// ��->head��
		pHashTableEnt[idx].pTail = pObj;	// ��->head��(tail)
		pHashTableEnt[idx].elmtCount = 1;
	}
	else {// ��->��->��...
		pObj->phNext = pHashTableEnt[idx].pHead;	//new  ��  -> �� head
		pHashTableEnt[idx].pHead->phPrev = pObj;	//new  �� <-> �� head
		pHashTableEnt[idx].pHead = pObj;			//head �� <-> ��
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

	//prev�� nullptr�̾���.
	Object* prev = target->phPrev;

	if (prev == NULL) {
		pFreeListHead = NULL;
		pFreeListTail = NULL;
	}
	else {
		prev->phNext = NULL;  //��(prev)->null, ��(target,tail)
		//pFreeListHead->phPrev = prev;
		pFreeListTail = prev;  //��(prev,tail), ��(target)

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

	if (pHashTableEnt[idx].pHead == pObj){ // 1. ����ʿ� ��ġ
		if (pHashTableEnt[idx].elmtCount == 1) { // ����ʿ� �����Ϸ��°� �ϳ��� ����
			pHashTableEnt[idx].pHead = NULL;
		}
		else{
			pHashTableEnt[idx].pHead = pObj->phNext;	//��(obj) <-> ��head <-> ��...
			pHashTableEnt[idx].pHead->phPrev = NULL;	//��(obj)  -> ��head <-> ��...
		}
	}
	else if (pHashTableEnt[idx].pTail == pObj) { // 2. �����ʿ� ��ġ
		if (pHashTableEnt[idx].elmtCount == 1) { // �����ʿ� �����Ϸ��°� �ϳ��� ����
			pHashTableEnt[idx].pTail = NULL;
		}
		else {
			pHashTableEnt[idx].pTail = pObj->phPrev;
			pHashTableEnt[idx].pTail->phNext = NULL;
		}
	}
	else {									// 3. �߾ӿ� ��ġ
		Object* target = obj;
		Object* prev = target->phPrev;					// ��prev <-> �� <-> ��next
		Object* next = target->phNext;
		prev->phNext = next;							// ��prev  -> ��next
		next->phPrev = prev;							// ��prev <-> ��next
	}


	pObj->phNext = NULL;
	pObj->phPrev = NULL;
	pHashTableEnt[idx].elmtCount--;
	return TRUE;
}

void InsertObjectIntoObjFreeList(Object* pObj) {
	pObj->phNext = NULL;	pObj->phPrev = NULL;

	if (pFreeListHead == NULL) {//����ִٸ� �ִ´�.
		pFreeListHead = pObj;
		pFreeListTail = pObj;
	}
	else {
		pFreeListHead->phPrev = pObj; // null<- ��(obj) <- ��(head)

		pObj->phNext = pFreeListHead;  //��(obj)<-> ��(head)

		pFreeListHead = pObj; // ��(obj,head)<->��
	}
}
