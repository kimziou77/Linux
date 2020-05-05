#include "Headers.h"

//TODO: 인자들을 통일시켜줄 수 없나?
void InsertThreadToReadyQueue(Thread *pThread){
	
	int priority = pThread->priority;
    //printf("현재 실행중인 스레드의 우선순위 : %d\n",priority);
	
    //Thread -> Ready Tail로 넣어줘야함
    printf("InsertThreadToReadyQueue %d (%d) \n",pThread->pid,pThread->priority);
	
    int tid = find_tid(pThread->pid);
	pThread->status = THREAD_STATUS_READY;

    if (pReadyQueueEnt[priority].pHead == NULL) {//암것도 없었을 때 그냥넣어준다.
		pReadyQueueEnt[priority].pHead = pThread;
		pReadyQueueEnt[priority].pTail = pThread;

		pReadyQueueEnt[priority].queueCount = 1;
	}
	else {
		//printf(" tail %d  tail->next %d \n",pReadyQueueEnt[priority].pTail->pid, pReadyQueueEnt[priority].pTail->phNext->pid);

		pReadyQueueEnt[priority].pTail->phNext = pThread;
		pThread->phPrev = pReadyQueueEnt[priority].pTail;
		pReadyQueueEnt[priority].pTail = pThread;
		pReadyQueueEnt[priority].queueCount++;
	}
	printf("%d(%d) ", pReadyQueueEnt[priority].queueCount,priority);
	Thread * tmp = pReadyQueueEnt[priority].pHead;
	for(int i=0;i<pReadyQueueEnt[priority].queueCount;i++){
		printf("%d(%d)->",tmp->pid,tmp->priority);
		tmp= tmp->phNext;
	}
	printf("\n");
	// print_pThreadEnt();
	// print_pReadyQueue();
	
	// print_pWaitingQueue();
	// print_pCurrentThread();

	// print_pThreadEnt();
	// print_pReadyQueue();
	// print_pWaitingQueue();
	// print_pCurrentThread();
	
    //printf("%d pReadyqueue[%d]의 개수 : %d\n",pThread->pid,priority,pReadyQueueEnt[priority].queueCount);
}
Thread *GetThreadByPid(int pid){//tid 아니고 pid로 가정
//ReadyQueue에서 위치를 알아온다. 포인터만 알아오면 된다.
    int tid = find_tid(pid);
    Thread * t = pThreadTbEnt[tid].pThread;
    Thread* cursor = pReadyQueueEnt[t->priority].pHead;

	while (cursor) {
		if (cursor->pid == pid)
			return cursor;
		cursor = cursor->phNext;
	}
	return NULL;
}

Thread *GetThreadFromWaitingQueue(int pid){
	//waiting queue로부터는 waiting이 끝난 애들을 데리고 와야한다.
    if(pWaitingQueueHead == NULL) return NULL;
    int tid = find_tid(pid);
    Thread * target = pThreadTbEnt[tid].pThread;
	
	if(target==pWaitingQueueHead){//target이 헤드일때
		Thread * next = target -> phNext;
		pWaitingQueueHead = target->phNext;
		next -> phPrev = NULL;
	}
	else if(target==pWaitingQueueTail){//target이 Tail일때
		Thread * prev = target->phPrev;
		pWaitingQueueTail = target->phPrev;
		prev->phNext=NULL;

	}
	else{	//중앙에 있으면
		Thread * next = target -> phNext;
		Thread * prev = target->phPrev;
		prev->phNext = next;
		next->phPrev = prev;
	}
    	
	target->phNext=NULL;
    target ->status = THREAD_STATUS_ZOMBIE;
    return target;
}
Thread * GetThreadFromReadyQueue(){
	//현재 가장 우선순위가 높은것의 주소를 넘겨준다. **연결은 되어있음
	for(int i=0;i<MAX_READYQUEUE_NUM;i++){
		if(pReadyQueueEnt[i].pHead!=NULL)
			return pReadyQueueEnt[i].pHead;
	}
	printf("레디큐에 암것도 없어요");
	return NULL;

}
BOOL DeleteThreadFromReadyQueue(Thread *pThread){
    //Waiting Queue로부터 Thread를 Delete 해준다.
    //아닌데 이거 주석 바꿔야 하는데 저것도 만들어줘야됨
    printf("DeleteThreadFromReadyQueue %d(%d) ",pThread->pid,pThread->priority);

	//no Object -> false;
	if (pThread == NULL) return FALSE;
    //이미 레디큐에 없으면 삭제할 필요가 없지
    if(GetThreadByPid(pThread->pid)==NULL) return FALSE;

    int pr=pThread->priority;
	//printf("헤드에 들어있는건 무엇일까요 : %d \n\n",pReadyQueueEnt[pr].pHead->pid);
	if (pReadyQueueEnt[pr].pHead == pThread){ // 1. 헤드쪽에 위치
		printf(" (head Delete ");
		if (pReadyQueueEnt[pr].queueCount== 1) { // 헤드쪽에 삭제하려는것 하나만 존재
			printf("1)\n");
			pReadyQueueEnt[pr].pHead = NULL;
		}
		else{
			printf("2)\n");
			pReadyQueueEnt[pr].pHead = pThread->phNext;
			pReadyQueueEnt[pr].pHead -> phPrev = NULL;
		}
	}
	else if (pReadyQueueEnt[pr].pTail == pThread) { // 2. 테일쪽에 위치
		printf(" (Tail Delete)\n");
		if (pReadyQueueEnt[pr].queueCount== 1) { // 테일쪽에 삭제하려는것 하나만 존재
			pReadyQueueEnt[pr].pTail = NULL;
		}
		else {
			pReadyQueueEnt[pr].pTail = pThread->phPrev;
			pReadyQueueEnt[pr].pTail->phNext = NULL;
		}
	}
	else {									// 3. 중앙에 위치
		printf("(Middle Delete)\n");
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
    //TODO: true 말고 pThread를 반환해줄 수는 없나?

	return TRUE;
}
void WaitingQueue_To_ReadyQueue(Thread * pThread){
    //Waiting-> Ready
	
	GetThreadFromWaitingQueue(pThread->pid);//waiting queue에서 삭제
    int pr = pThread->priority;
	InsertThreadToReadyQueue(pThread);//ReadyQueue의 Tail로 넣는다.
    
    pReadyQueueEnt[pr].queueCount++;
	pThread->status = THREAD_STATUS_READY;
}
void InsertThreadToWaitingQueue(Thread *pThread){
	//wating queue의 tail로 넣는 함수

    printf("%d 를 Waiting Queue에 넣습니다\n",pThread->pid);
    int tid = find_tid(pThread->pid);

    if (pWaitingQueueHead == NULL) {//head insert
		printf("insertWaiting @\n");
		pWaitingQueueHead = pThread;
		pWaitingQueueTail = pThread;
	}
	else {//tail insert
		printf("insertWaiting @@\n");
		pWaitingQueueTail->phNext = pThread;
		pThread->phPrev = pWaitingQueueTail;
		pWaitingQueueTail= pThread;
	}
	printf("insertWaiting @@@@\n");
	pThread->status = THREAD_STATUS_WAIT;
}
void DeleteThreadFromWaitingQueue(Thread *pThread){//사실 이건 필요가 없는데.. 
	printf("Delete Thread From Waiting Queue : %d \n",pThread->pid);
	GetThreadFromWaitingQueue(pThread->pid);
}
//delete From Waiting queue함수 만들기
