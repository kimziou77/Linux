#include "hw1.h"
#include <malloc.h>

#define INSERT_OBJECT_SIZE (48)

void InitFreeList(){

    int i;
    Object* pObject;

    for(i=0;i<INSERT_OBJECT_SIZE;i++){
        pObject=(Object*)malloc(sizeof(Object));
        pObject->objnum = OBJ_INVALID;
        InsertObjectIntoObjFreeList(pObject);
    }
}

void printHashTable(){

    int i,j;

    printf("-----------HashTable-----------\n");

    for(i=0;i<HASH_TBL_SIZE;i++){
        printf("index: %d |",i);

        Object* temp=pHashTableEnt[i].pHead;
        for(j=0;j<pHashTableEnt[i].elmtCount;j++){
            printf(" %d",temp->objnum);
            temp=temp->phNext;
        }
        printf("\n");
    }
}

void DeleteProcess(int idx){

    Object* temp=GetObjectByNum(idx);
	DeleteObject(temp);
    temp->objnum = OBJ_INVALID;
    InsertObjectIntoObjFreeList(temp);
}

void main(){

    int i;

    Init();
    InitFreeList();

    //testcase1
    for(i=0;i<INSERT_OBJECT_SIZE;i++){
        if(i%2==0)
            InsertObjectToTail(GetObjectFromObjFreeList(),i);
        else
            InsertObjectToHead(GetObjectFromObjFreeList(),i);
    }

    printf("case1) Insert Input\n");
    printHashTable();

    for(i=0;i<INSERT_OBJECT_SIZE;i++){
        DeleteProcess(i);
    }

    printf("case1) Delete All\n");
    printHashTable();

    //testcase2
    for(i=0;i<INSERT_OBJECT_SIZE;i++){
        InsertObjectToTail(GetObjectFromObjFreeList(),i);
    }

    for(i=8;i<16;i++){
        DeleteProcess(i);
        InsertObjectToHead(GetObjectFromObjFreeList(),i);
    }
    for(i=32;i<40;i++){
        DeleteProcess(i);
        InsertObjectToHead(GetObjectFromObjFreeList(),i);
    }
    printf("case2)\n");
    printHashTable();
}
