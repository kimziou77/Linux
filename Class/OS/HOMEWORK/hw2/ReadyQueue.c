#include "Headers.h"

void InsertThreadToReadyQueue(Thread *pNewThread, int priority){
    printf("현재 실행중인 스레드의 우선순위 : %d\n",priority);
    //Thread -> Ready Tail로 넣어줘야함
    printf("InsertThreadToReadyQueue\n");
    int tid = find_tid(pNewThread->pid);
    if (pReadyQueueEnt[priority].pHead == NULL) {
		pReadyQueueEnt[priority].pHead = pNewThread;
		pReadyQueueEnt[priority].pTail = pNewThread;

		pReadyQueueEnt[priority].queueCount = 1;
	}
	else {
		pReadyQueueEnt[priority].pTail->phNext = pNewThread;
		pNewThread->phPrev = pReadyQueueEnt[priority].pTail;
		pReadyQueueEnt[priority].pTail = pNewThread;
		pReadyQueueEnt[priority].queueCount++;
	}
    printf("pReadyqueue[%d]의 개수 : %d\n",priority,pReadyQueueEnt[priority].queueCount);
    
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
Thread *GetThreadFromWaitingQueue(){
    //Waiting Queue의 head로부터 Thread를 가지고 온다.
    //TODO:그은데 waiting queue에서 thread를 가지고 올 필요가 있나?
    //알아서 시그널로 전송되어서 레디로 옮겨오지 않나?
    if(pWaitingQueueHead == NULL) return NULL;
    
    Thread * target = pWaitingQueueHead;

    Thread * next = target -> phNext;
    next->phPrev = NULL;
    target->phNext = NULL;//서로 이어진걸 끊어놓기

    pWaitingQueueHead = next;
    return target;
}
BOOL DeleteThreadFromReadyQueue(Thread *pThread){
    //Waiting Queue로부터 Thread를 Delete 해준다.
    //아닌데 이거 주석 바꿔야 하는데 저것도 만들어줘야됨
    printf("DeleteThreadFromReadyQueue %d \n",pThread->pid);

	//no Object -> false;
	if (pThread == NULL) return FALSE;
    //이미 레디큐에 없으면 삭제할 필요가 없지
    if(GetThreadByPid(pThread->pid)==NULL) return FALSE;

    int pr=pThread->priority;
	if (pReadyQueueEnt[pr].pHead == pThread){ // 1. 헤드쪽에 위치
		if (pReadyQueueEnt[pr].queueCount== 1) { // 헤드쪽에 삭제하려는것 하나만 존재
			pReadyQueueEnt[pr].pHead = NULL;
		}
		else{
			pReadyQueueEnt[pr].pHead = pThread->phNext;
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
    //TODO: true 말고 pThread를 반환해줄 수는 없나?
	return TRUE;
}
void WaitingQueue_To_ReadyQueue(Thread * pThread){
    //Waiting-> Ready
    int pr = pThread->priority;
    //그냥 ready큐에 일단 넣어두고 웨이팅에서 삭제해버리자
    //ReadyQueue의 Tail로 넣는다.
    
    pReadyQueueEnt[pr].queueCount++;
    
    //FreeList의 head로 넣는다.???
    //ReadyQueue의 head로 넣는다.
    //내개ㅏ 해야할건 Tail로 넣는것일텐데
}

//wating queue의 tail로 넣는 함수

//해당 스레드를 waiting queue에서 빼네는 함수.
