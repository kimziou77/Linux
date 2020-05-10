#include "Headers.h"

int thread_create(thread_t * thread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg)
{
    if(DEBUGGING) printf("[+] thread_create\n");

    char* pStack;
    pStack= malloc(STACK_SIZE);
    int flags= SIGCHLD|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_VM;
    thread_t pid = clone(start_routine,(char*)pStack+STACK_SIZE,flags,arg); 
    kill(pid,SIGSTOP);//생성 후 stop
    //Allocate TCB
    Thread* childThread = (Thread*)malloc(sizeof(Thread));
    childThread->pid=pid;
    childThread->priority=priority;
    childThread->stackSize=STACK_SIZE;
    childThread->stackAddr=pStack;//TODO: 주소가 이게 맞나?
    childThread->exitCode = NON_EXIT;

    *thread = FindEmptyThreadTable();//빈 TCB table을 찾아 id 값 할당.
    pThreadTbEnt[*thread].pThread = childThread;

    if(pCurrentThread == NULL){//현재 실행중인 스레드가 없다면
        childThread->status=THREAD_STATUS_RUN;//바로 실행.
        pCurrentThread = childThread;
        __ContextSwitch(NONE,childThread->pid);
    }
    else if(childThread->priority >= pCurrentThread->priority){
        if(DEBUGGING) printf("현재 스레드가 더 우선순위가 높거나 같음\n");
        if(DEBUGGING) printf("따라서 새로운 스레드는 레디큐에 들어감\n");
        childThread->status=THREAD_STATUS_READY;//Set child Thread status to Ready
        InsertThreadToReadyQueue(childThread);//Move TCB to ready queue
    }
    else{
        if(DEBUGGING) printf("새로운 스레드의 우선순위가 더 높음 바로 컨텍스트 스위칭\n");
        InsertThreadToReadyQueue(pCurrentThread);
        pCurrentThread->status = THREAD_STATUS_READY;
        __ContextSwitch(pCurrentThread->pid,childThread->pid);
    }
    if(DEBUGGING)   printf("[-] thread_create\n");
    return *thread;// 생성한 프로세스의 tid값 반환
}

int thread_suspend(thread_t tid){
    if(DEBUGGING)   printf("[+] thread_suspend\n");
    if(pThreadTbEnt[tid].bUsed==FALSE) return FAILED;//멈추려는 스레드가 존재하지 않으면 FAILED
    Thread * pThread = pThreadTbEnt[tid].pThread;//멈출 대상
        if(pThread->status==THREAD_STATUS_RUN){ //RUN
        if(DEBUGGING) printf("suspend @ RUN\n");
        InsertThreadToWaitingQueue(pThread);//Move TCB to waiting queue
        pThread->status = THREAD_STATUS_WAIT;//Set status to waiting
        kill(mainPid,SIGALRM);
    }
    else if(pThread->status==THREAD_STATUS_READY){  //READY
        if(DEBUGGING) printf("suspend @@ READY\n");
        DeleteThreadFromReadyQueue(pThread);
        InsertThreadToWaitingQueue(pThread);//Move TCB to waiting queue
        pThread->status= THREAD_STATUS_WAIT;//Set thread status to waiting
    }
    else if(pThread->status==THREAD_STATUS_WAIT){// WAIT
        if(DEBUGGING) printf("suspend @@@ WAIT");
    }
    
    if(DEBUGGING)   printf("[-] thread_suspend\n");
    return SUCCESS;
}
int thread_cancel(thread_t tid)//해당 스레드를 terminate 시키는 함수
{
    if(DEBUGGING) printf("[+] thread_cancel\n");
    Thread * targetThread = pThreadTbEnt[tid].pThread;
    if(targetThread == NULL){
        if(DEBUGGING) printf("지정한 타겟이 존재하지 않습니다\n");
        return FAILED;
    }

    kill(targetThread->pid, SIGKILL);
    if(targetThread->status == THREAD_STATUS_READY){
        if(DEBUGGING) printf("targetThread가 readyQueue에 있었습니다\n");
        DeleteThreadFromReadyQueue(targetThread);
    }
    else if(targetThread->status == THREAD_STATUS_WAIT){
        if(DEBUGGING) printf("targetThread가 waiting큐에 있었습니다.\n");
        DeleteThreadFromWaitingQueue(targetThread);
    }
    else{
        if(DEBUGGING) printf("좀비상태나, 실행상태, cancel 대상이 아닌것 같습니다\n");
        return FAILED;
    }
    targetThread->status = THREAD_STATUS_ZOMBIE;
    pThreadTbEnt[tid].bUsed = FALSE;
    pThreadTbEnt[tid].pThread = NULL;
    free(targetThread->stackAddr);
    free(targetThread);

    if(DEBUGGING) printf("[-] thread_cancel\n");
    return SUCCESS;
}

int thread_resume(thread_t tid)
{
    if(DEBUGGING)   printf("[+] thread_resume\n");
    Thread* targetThread = pThreadTbEnt[tid].pThread;

     if(targetThread->priority < pCurrentThread->priority){//resume대상이 더 우선순위가 높으면
        InsertThreadToReadyQueue(pCurrentThread);
        pCurrentThread->status = THREAD_STATUS_READY;
        WaitingQueue_To_ReadyQueue(targetThread);
        targetThread->status=THREAD_STATUS_READY;
        kill(mainPid,SIGALRM);//둘 다 레디큐에 넣고 메인에게 컨텍스트요청
    }
    else{
        targetThread->status =THREAD_STATUS_READY;
        WaitingQueue_To_ReadyQueue(targetThread);
    }

    if(DEBUGGING)   printf("[-] thread_resume\n");
    return SUCCESS;
}

thread_t thread_self()
{
    if(DEBUGGING) printf("[+] thread_self\n");
    int pid= getpid();
    for(int i=0;i < MAX_THREAD_NUM;i++){
        if(pThreadTbEnt[i].pThread->pid==pid){
            if(DEBUGGING) printf("[-] thread_self\n");
            return i;// thread ID
        }
    }
    if(DEBUGGING) printf("NONE ID \n");
    if(DEBUGGING) printf("[-] thread_self\n");
}

int thread_join(thread_t tid, void * * retval){
    if(DEBUGGING) printf("[+] thread_join\n");
    if(DEBUGGING) printf("child : %d 대기\n",pThreadTbEnt[tid].pThread->pid);
    thread_t pTid = thread_self();
    Thread * pThread = pThreadTbEnt[pTid].pThread;
    Thread * cThread = pThreadTbEnt[tid].pThread;
    if(cThread->status != THREAD_STATUS_ZOMBIE){
        if(DEBUGGING) printf("자식프로세스가 종료된상태가 아니므로 대기합니다\n");
        pThread->status = THREAD_STATUS_WAIT;
        InsertThreadToWaitingQueue(pThread);
        pCurrentThread = NULL;//TODO: NULL

        if(DEBUGGING) printf("대기하는동안 돌아갈 프로세스를 가져옵니다.\n");
        Thread * nThread = GetThreadFromReadyQueue();
        if(nThread!=NULL){
            if(DEBUGGING) printf("레디큐로부터 새로운 프로세스를 가져오는것에 성공했습니다.\n");
            DeleteThreadFromReadyQueue(nThread);
            nThread->status = THREAD_STATUS_RUN;
            pCurrentThread = nThread;//부모 -> new프로세스
            kill(mainPid, SIGALRM);//부모한테 컨텍스트스위칭 요청
            //kill(mainPid,SIGUSR1);//다시 2초 예약
            //__ContextSwitch(NONE,nThread->pid);


            if(DEBUGGING) printf("이제 자식스레드가 좀비일때까지 pause()\n");
            while(cThread->status != THREAD_STATUS_ZOMBIE) pause();
        }
        else{
            if(DEBUGGING) printf("가져올 프로세스가 존재하지 않습니다.\n");
        }
    }
    else{
        if(DEBUGGING) printf("자식프로세스가 이미 종료되었습니다.\n");
    }
    if(DEBUGGING) printf("%d 자식프로세스가 좀비가 되었습니다! %d 부모가 일어납니다.\n",cThread->pid, pThread->pid);

    *retval = (int *)(&(cThread->exitCode));//Get child's TCB Put exitCode into retVal
    Reaping(cThread->pid);

    if(DEBUGGING) printf("[-] thread_join\n");
    return SUCCESS;
  
}
void Reaping(int cPid){
    if(DEBUGGING) printf("[+] Reaping\n");
    int pTid = thread_self();
    int cTid = find_tid(cPid);
    Thread * pThread = pThreadTbEnt[pTid].pThread;
    Thread * cThread = pThreadTbEnt[cTid].pThread;
    DeleteThreadFromWaitingQueue(pThread);

    if(pThread != pCurrentThread)
    {



    if(cThread==NULL){
        if(DEBUGGING) printf("child Thread NULL\n");
        return;
    }
    //현재 스레드랑 비교
    if(pCurrentThread == NULL){
        if(DEBUGGING) printf("현재 실행중인 프로세스가 없으면 부모가 다시 돌아간다.\n");
        //pCurrentThread = pThread;
        //pThread->status = THREAD_STATUS_RUN;
        InsertThreadToReadyQueue(pThread);
        pCurrentThread = NULL;
        kill(mainPid,SIGALRM);
        kill(getpid(), SIGSTOP);
        // kill(mainPid,SIGUSR1);//다시 2초 예약
        // __ContextSwitch(NONE, pThread->pid);
    }
    else if(pThread->priority < pCurrentThread->priority){
        if(DEBUGGING) printf("부모가 현재 실행중인 프로세스보다 우선순위가 높다면 부모가 돌아간다.\n");
        // pCurrentThread = pThread;
        // pThread->status = THREAD_STATUS_RUN;
        InsertThreadToReadyQueue(pThread);
        pCurrentThread = NULL;
        kill(mainPid,SIGALRM);
        kill(getpid(), SIGSTOP);
        // kill(mainPid,SIGUSR1);//다시 2초 예약
        // __ContextSwitch(NONE, pThread->pid);
    }
    else{
        InsertThreadToReadyQueue(pThread);
        if(DEBUGGING) printf("현재 돌아가고 있는 프로세스가 부모보다 우선순위가 높으면 부모는 컨텍스트 스위칭을 기다린다.\n");
    }
    }
    if(DEBUGGING) print_all();
    //reaping

    DeleteThreadFromWaitingQueue(cThread);
    if(DEBUGGING) printf("DeleteThreadFromWaitingQueue End 1\n");
    pThreadTbEnt[cTid].bUsed = FALSE;
    pThreadTbEnt[cTid].pThread = NULL;
if(DEBUGGING) printf("DeleteThreadFromWaitingQueue End2 \n");
    free(cThread->stackAddr);
    free(cThread);
if(DEBUGGING) printf("DeleteThreadFromWaitingQueue End 3\n");
    if(DEBUGGING) printf("[-] Reaping\n");
}
int thread_exit(void * retval){
   if(DEBUGGING) printf("[+] thread_exit\n");

   Thread * eThread = pCurrentThread;// 좌우 포인터

    eThread->exitCode = *((int*)retval);
    InsertThreadToWaitingQueue(eThread);//waiting queue에 넣는다.
    eThread -> status = THREAD_STATUS_ZOMBIE;
    pCurrentThread = NULL;
   if(DEBUGGING) printf("[-] thread_exit\n");
   exit(0);
}
