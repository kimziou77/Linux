#include "MsgQueue.h"
#include "Headers.h"

//메시지 큐를 사용하는 스레드의 실행을 제어한다.
pmqd_t pmq_open(const char* name, int flags, mode_t perm, pmq_attr* attr)
{
    if(DEBUGGING) printf("[+] pmq_open %s\n",name);
    /*메세지큐를 생성하거나 이미 존재하는 mq 오픈한다.*/
    pmqd_t existNum = FindQCBTableName(name);
    if(existNum != FAILED) {
        qcbTblEntry[existNum].openCount++;
        if(DEBUGGING) printf("[-] pmq_open 이미존재@\n");
        return existNum;//존재하는 번호라면 return
    }
    pmqd_t qcbTnum = FindEmptyQCBTableNum();
    if(qcbTnum==FAILED) return FAILED;//빈 곳이 없으면 FAILED
    
    //qcbTnum번째 QCB Table에 QCB 생성하고 초기화한다.
    /*qcb 생성*/
    Qcb * qcb = malloc(sizeof(Qcb));
    qcb->msgCount=0;
    qcb->pMsgHead=NULL;
    qcb->pMsgTail=NULL;
    qcb->waitThreadCount=0;
    qcb->pWaitQHead=NULL;
    qcb->pWaitQTail=NULL;
    
    /*QCB Table Entry 초기화*/
    strcpy(qcbTblEntry[qcbTnum].name,name);
    qcbTblEntry[qcbTnum].mode = perm;
    qcbTblEntry[qcbTnum].bUsed = TRUE;
    qcbTblEntry[qcbTnum].openCount = 1;
    qcbTblEntry[qcbTnum].pQcb = qcb;
    //TODO: attr?
    if(DEBUGGING) printf("[-] pmq_open 테이블 생성@@ %d\n",qcbTnum);
    return qcbTnum; //mq descriptor
}

int pmq_close(pmqd_t mqd)
{
    if(qcbTblEntry[mqd].openCount>0){
        qcbTblEntry[mqd].openCount--;
    }
    else if(qcbTblEntry[mqd].openCount <= 0){ //TODO: 1??
        qcbTblEntry[mqd].bUsed=FALSE;
        memset(qcbTblEntry[mqd].name,0,sizeof(PMQ_NANE_LEN_MAX));
        qcbTblEntry[mqd].pQcb=NULL;
    }
    else{
        if(DEBUGGING) printf("[-] pmqClose error\n");
        return FAILED;
    }
    return SUCCESS;
}

int pmq_send(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int msg_prio)
{

    if(DEBUGGING) printf("[+] pmq_send\n");
    /*접근하고자 하는 우체통 qcb를 가져온다.*/
    
    if(qcbTblEntry[mqd].bUsed==FALSE) return FAILED;
    Qcb* qcb = qcbTblEntry[mqd].pQcb;
    
    /*메시지 초기화 */
    Message * message = malloc(sizeof(Message));
    strcpy(message->data,msg_ptr);//메시지 복사!!!!
    if(DEBUGGING) printf("메시지 복사 완료\n");
    message->size = msg_len;//메시지 길이
    message->priority = msg_prio;//메시지 우선순위
    
    /*우선순위에 따라 Message 리스트에 넣는다.*/
    if(qcb->msgCount==0){//아무것도 없었다면 head == tail
        if(DEBUGGING) printf("메시지가 현재 아무것도 없었음.\n");
        if(DEBUGGING) printf("[=] head==tail %d\n",msg_prio);
        
        qcb->pMsgHead = message;
        qcb->pMsgTail = message;
        qcb->msgCount++;
    }
    else{//tail에 넣는다. 단, 우선순위 고려
        if(DEBUGGING) printf("메시지가 존재해서 tail에 insert 한다 %d\n",qcb->msgCount);
        if(DEBUGGING) printf("[=] tail insert%d\n",qcb->pMsgHead->priority);
        BOOL flag = FALSE;
        Message * pPrev;

        for(Message *cursor = qcb->pMsgHead ; cursor!=NULL; ){
                
            if(cursor->priority < msg_prio){//자신보다 낮은 우선순위라면 삽입한다.
                
                if(DEBUGGING) printf("자신(%d)보다 우선순위 낮은 친구(%d) 발견했음\n", msg_prio,cursor->priority);
                if(DEBUGGING) printf("이전 순서에 삽입한다\n");
                
                if(cursor == qcb->pMsgHead){//현재 가리키는 친구가 헤드라면
                    cursor->pPrev = message;//이전 순서에 삽입
                    qcb->pMsgHead = message;
                    message->pNext = cursor;
                    message->pPrev = NULL;
                }
                else{//중간에 있었던 친구였다면
                    cursor->pPrev = message;//이전 순서에 삽입
                    pPrev->pNext = message;//이전 친구가 있을거임
                    message->pNext = cursor;
                    message->pPrev = pPrev;
                }
                flag = TRUE;
                break;
            }
            else{
                if(DEBUGGING) printf("우선순위가 높지 않음?\n");
            }
            pPrev = cursor;
            cursor = cursor->pNext;
            
        }
        if(!flag){//NULL이 될 때 까지 자신보다 낮은 우선순위를 발견하지 못함.
            if(DEBUGGING) printf("우선순위 젤 꼴지임\n");
            pPrev->pNext = message;
            message->pPrev = pPrev;
            message->pNext = NULL;
        }
        qcb->msgCount++;
    }
    
    //Message를 집어넣었는데
    //Thread 리스트에서 대기 쓰레드를 가져온다.
    if(DEBUGGING) printf("이제 메시지는 보냈고, 대기중인 스레드가 있는지 확인할것임\n");
    if(qcb!=NULL && qcb->waitThreadCount > 0){//대기하고 있는 스레드가 있다면
        if(DEBUGGING) printf("대기중인 스레드가 있음 count : %d\n",qcb->waitThreadCount);
        if(DEBUGGING) print_all();
        //찾은 thread를 레디큐의 tail로 보낸다.

        Thread * qcbThread = qcb->pWaitQHead;
        

        if(qcb->waitThreadCount==1){//Head하나만 있었다는 뜻

         if(DEBUGGING) printf("대기중인 스레드가 하나임\n");

            WaitingQueue_To_ReadyQueue(qcbThread);//Count가 0이상이면 널일리가 없음
            if(DEBUGGING) print_all();
            qcb->pWaitQHead=NULL;
            qcb->pWaitQTail=NULL;//qcb에 이제 남아있는 waitThread없음
            qcbThread->phNext=NULL;
            qcbThread->phPrev=NULL;
        }
        else{//여러개 있었음.
        if(DEBUGGING) printf("대기중인 스레드가 여러개임\n");
            
            qcb->pWaitQHead = qcb->pWaitQHead->phNext;//헤드 변경

            // pWaitingQueueHead->phNext = qcb->pWaitQHead;
            // qcb->pWaitQHead->phPrev = NULL;//헤드의 앞은 null

            DeleteThreadFromWaitingQueue(qcbThread);
            InsertThreadToReadyQueue(qcbThread);//전 헤드 레디큐에 넣기

            // qcb->pWaitQHead->phPrev=NULL;//헤드니까 앞에는 없음
            // qcbThread->phNext=NULL;
            // qcbThread->phPrev=NULL;
            
        }
        qcb->waitThreadCount--;
    }
    else{
        if(DEBUGGING) printf("대기중인 스레드가 없음\n");
    }

    if(DEBUGGING) printf("[-] pmq_send\n");
    return SUCCESS;

}
ssize_t pmq_receive(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int* msg_prio)
{
    //무조건 헤드에서 먼저 받는다.
    if(DEBUGGING) printf("[+] pmq_recieve %d\n",mqd);
    if(DEBUGGING) print_all();
    /*접근하고자 하는 우체통 qcb를 가져온다.*/
    if(qcbTblEntry[mqd].bUsed==FALSE)
        return FAILED;
    Qcb* qcb = qcbTblEntry[mqd].pQcb;
    if(qcb!=NULL){
        printf("nkkk");
    }
    else{
        printf("g0g0g0\n");
    }
    

    if(qcb->msgCount==0){/*Message queue에 가져올 Message가 없음.*/
        /* Waiting queue에 넣는다.*/
        
        if(DEBUGGING) printf("messagequeue에서 가져올 메시지가 없음 대기상태로 변경 %d\n",qcb->msgCount);

        if(qcb->waitThreadCount==0){
            if(DEBUGGING) printf("현재 대기중인 스레드가 없음 qcb웨잇큐 헤드에 현재스레드 넣는다.\n");
            qcb->pWaitQHead = pCurrentThread;
            qcb->pWaitQTail = pCurrentThread;
            InsertThreadToWaitingQueue(pCurrentThread);
        }
        else{
            if(DEBUGGING) printf("대기중인 스레드가 존재 qcb웨잇 테일에 현재스레드 넣는다.\n");
            qcb->pWaitQTail->phNext = pCurrentThread;
            pCurrentThread->phPrev = qcb->pWaitQTail;
            qcb->pWaitQTail = pCurrentThread;
            pCurrentThread->status = THREAD_STATUS_WAIT;
            InsertThreadToWaitingQueue(pCurrentThread);
        }
        qcb->waitThreadCount++;

        thread_t tid = thread_self();

        pCurrentThread=NULL;
        if(DEBUGGING) printf("이제 이 스레드는 중단됩니다.\n");
        kill(mainPid,SIGALRM);
        kill(getpid(),SIGSTOP);
        // Thread * nThread = GetThreadFromReadyQueue();
        // nThread->status = THREAD_STATUS_RUN;
        // __ContextSwitch(getpid(),nThread->pid);
        if(DEBUGGING) printf("리시브할 메시지가 있어서 레디큐에 왔다가 살아난 친구임니다 %d\n",getpid());
        //컨텍스트 스위칭을 하고 나서 잠들어야 한다는데 ㅜ ㅜ

        //qcb->pWaitQTail;
        //suspend하는게 리시브에서 중요한 메커니즘 
        //TODO: 스레드를 waiting queue !! Tail !!! 에 넣는다.
        //ready queue에 있는 것을 실행한다.
    }

        /*Message가져오기*/
        Message * message;
        if(qcb->msgCount>0){
            message = qcb->pMsgHead; //헤드에서 메시지 가져오기
            qcb->msgCount--;

            qcb->pMsgHead = message->pNext; //다음 메시지를 헤드로 만들기
            qcb->pMsgHead -> pPrev = NULL; //Head 삭제로 인한 prev NULL처리    
            message->pNext= NULL; //받아온 메시지에 연결된 next 연결고리도 끊어준다.
        }
        
        
        // if(message!=NULL){
        //     if(DEBUGGING) printf("message는 널이 아닙니다\n");
            
        //     if(DEBUGGING) printf("이제부터 next 가 head입니다\n");
        //     if(qcb->pMsgHead !=NULL){
        //         if(DEBUGGING) printf("메세지 헤드가 널이 아닙미다 \n");
        //         qcb->pMsgHead -> pPrev = NULL; //Head 삭제로 인한 prev NULL처리    
        //     }
        //     else{
        //         if(DEBUGGING) printf("메시지 헤드가 널입미다.\n");
        //     }
                
        //     message->pNext= NULL; //받아온 메시지에 연결된 next 연결고리도 끊어준다.
        // }
        // else{
        //     if(DEBUGGING) printf("message가 널입미다\n");
        // }


        int len = (message->size > msg_len) ? msg_len : message->size;
        strncpy(msg_ptr,message->data, len);
        *msg_prio = message->priority;//priority 
        if(DEBUGGING) printf("[-] pmq_recieve\n");
        return len;//짧은 것의 길이 (읽은 만큼 반환하기)
    
}
