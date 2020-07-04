#include "Headers.h"

void InsertThreadToReadyQueue(Thread *pThread){

	if(pThread->status == THREAD_STATUS_READY) return;

	int priority = pThread->priority;
    //Thread -> Ready Tail로 넣어줘야함
	
    int tid = find_tid(pThread->pid);
	pThread->status = THREAD_STATUS_READY;

    if (pReadyQueueEnt[priority].pHead == NULL) {//암것도 없었을 때 그냥넣어준다.
		pReadyQueueEnt[priority].pHead = pThread;
		pReadyQueueEnt[priority].pTail = pThread;

		pReadyQueueEnt[priority].queueCount = 1;
	}
	else {
		pReadyQueueEnt[priority].pTail->phNext = pThread;
		pThread->phPrev = pReadyQueueEnt[priority].pTail;
		pReadyQueueEnt[priority].pTail = pThread;
		pReadyQueueEnt[priority].queueCount++;
	}
}
Thread *GetThreadByPid(int pid){//tid 아니고 pid로 가정
//ReadyQueue에서 위치를 알아온다. 포인터만 알아오면 된다.
    int tid = find_tid(pid);
	
    Thread * t = pThreadTblEnt[tid].pThread;
    Thread* cursor = pReadyQueueEnt[t->priority].pHead;

	while (cursor) {
		if (cursor->pid == pid)
			return cursor;
		cursor = cursor->phNext;
	}
	return NULL;
}

Thread *GetThreadFromWaitingQueue(int pid){
	if(DEBUGGING) printf("GET thread from wating queue\n");

    if(pWaitingQueueHead == NULL) return NULL;
    int tid = find_tid(pid);
    Thread * target = pThreadTblEnt[tid].pThread;

	if(target->status != THREAD_STATUS_WAIT && target->status != THREAD_STATUS_ZOMBIE)
	{
		return target;
	}

	
	if(pWaitingQueueHead == pWaitingQueueTail){//하나만 있을때
		if(DEBUGGING ) printf("저 혼자만 이써요 ㅜㅜ\n");
		pWaitingQueueHead= pWaitingQueueTail =NULL;
	}
	else if(target==pWaitingQueueHead){//target이 헤드일때
		if(DEBUGGING ) printf("head입니다\n");
		Thread * next = target -> phNext;
		if(next!=NULL){
			pWaitingQueueHead = next;
			next -> phPrev = NULL;
		}
		else{
			pWaitingQueueHead=NULL;
		}
	}
	else if(target==pWaitingQueueTail){//target이 Tail일때
		if(DEBUGGING) printf("tail입니다\n");
		Thread * prev = target->phPrev;
		pWaitingQueueTail = prev;
		if(prev!=NULL)
			prev->phNext=NULL;
	}
	else{	//중앙에 있으면
	// printf("target  Middle\n");
		if(DEBUGGING) printf("중앙에이써요@@@@\n");
		Thread * next = target -> phNext;
		Thread * prev = target->phPrev;
		// printf("@@@@ 1\n");
		prev->phNext = next;
		// printf("@@@@ 2\n");
		next->phPrev = prev;
		// printf("@@@@ End\n");
	}
    
	target->phNext=NULL;
	target->phPrev=NULL;
	
    target ->status = THREAD_STATUS_ZOMBIE;
	// printf("웨이팅큐에서 삭제 성공\n");
    return target;
}
Thread * GetThreadFromReadyQueue(){
	//현재 가장 우선순위가 높은것의 주소를 넘겨준다. **연결은 되어있음
	

	for(int i=0;i<MAX_READYQUEUE_NUM;i++){
		if(pReadyQueueEnt[i].pHead != NULL)
			return pReadyQueueEnt[i].pHead;
	}
	return NULL;

}
BOOL DeleteThreadFromReadyQueue(Thread *pThread){
    //ReadyQueue로부터 Thread를 Delete 해준다.
	
	if (pThread == NULL) return FALSE;
    //이미 레디큐에 없으면 삭제할 필요가 없지

	Thread * pidP = GetThreadByPid(pThread->pid);
    if(pidP==NULL) return FALSE;
	int pid = pidP->pid;

    int pr=pThread->priority;
	//printf("헤드에 들어있는건 무엇일까요 : %d \n\n",pReadyQueueEnt[pr].pHead->pid);
	if (pReadyQueueEnt[pr].pHead == pThread){ // 1. 헤드쪽에 위치
		if (pReadyQueueEnt[pr].queueCount== 1) { // 헤드쪽에 삭제하려는것 하나만 존재
			pReadyQueueEnt[pr].pHead = NULL;
		}
		else{
			// printf("pHead pid : %d, NextPid : %d\n",pReadyQueueEnt[pr].pHead->pid, pReadyQueueEnt[pr].pHead->phNext->pid);
			pReadyQueueEnt[pr].pHead = pThread->phNext;
			// pThread->phNext->phPrev=NULL;
			pReadyQueueEnt[pr].pHead -> phPrev = NULL;
		}
	}
	else if (pReadyQueueEnt[pr].pTail == pThread) { // 2. 테일쪽에 위치
		if (pReadyQueueEnt[pr].queueCount== 1) { // 테일쪽에 삭제하려는것 하나만 존재
			pReadyQueueEnt[pr].pTail = NULL;
		}
		else {
			pReadyQueueEnt[pr].pTail = pThread->phPrev;
			pReadyQueueEnt[pr].pTail->phNext = NULL;
		}
	}
	else {									// 3. 중앙에 위치
		Thread* target = pThread;
		Thread* prev = target->phPrev;
		Thread* next = target->phNext;
		prev->phNext = next;
		next->phPrev = prev;
	}
	pThread->phNext = NULL;
	pThread->phPrev = NULL;
	pReadyQueueEnt[pr].queueCount--;
	
	pThread->status = THREAD_STATUS_ZOMBIE;
	//printf("%d pReadyqueue[%d]의 개수 : %d\n",pThread->pid,pThread->priority,pReadyQueueEnt[pThread->priority].queueCount);
	if(DEBUGGING) printf("[+] Delete Thread From Readys Queue \n");
	return TRUE;
}
void WaitingQueue_To_ReadyQueue(Thread * pThread){
    //Waiting-> Ready
	DeleteThreadFromWaitingQueue(pThread);//waiting queue에서 삭제
	InsertThreadToReadyQueue(pThread);//ReadyQueue의 Tail로 넣는다.
	pThread->status = THREAD_STATUS_READY;
	
}
void InsertThreadToWaitingQueue(Thread *pThread){
	//wating queue의 tail로 넣는 함수
	// printf("Insert Thread To WaitingQueue %d(%d)\n ",pThread->pid,pThread->priority);
    thread_t tid = find_tid(pThread->pid);
	
    if (pWaitingQueueHead == NULL) {//head insert
		// printf("insertWaiting @\n");
		pWaitingQueueHead = pThread;
		pWaitingQueueTail = pThread;
	}
	else {//tail insert
		// printf("insertWaiting @@\n");
		pWaitingQueueTail->phNext = pThread;
		pThread->phPrev = pWaitingQueueTail;
		pWaitingQueueTail= pThread;
	}
	// printf("insertWaiting @@@@\n");
	pThread->status = THREAD_STATUS_WAIT;
}
void DeleteThreadFromWaitingQueue(Thread *pThread){//이름과 인자의 통일성을 위해..
	// printf("Delete Thread From Waiting Queue : %d \n",pThread->pid);
	
	GetThreadFromWaitingQueue(pThread->pid);
}
BOOL IsReadyQueueEmpty(){
	
	for(int i=0;i<MAX_READYQUEUE_NUM;i++){
		if(pReadyQueueEnt[i].pHead != NULL)
			return FALSE;
	}
	
	return TRUE;
}
