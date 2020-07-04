#include "MsgQueue.h"
#include "Headers.h"

//메시지 큐를 사용하는 스레드의 실행을 제어한다.
pmqd_t pmq_open(const char* name, int flags, mode_t perm, pmq_attr* attr)
{
    /*메세지큐를 생성하거나 이미 존재하는 mq 오픈한다.*/
    pmqd_t existNum = FindQCBTableName(name);
    if(existNum != FAILED) {
        qcbTblEntry[existNum].openCount++;
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

    return qcbTnum; //mq descriptor
}

int pmq_close(pmqd_t mqd)
{
    if(qcbTblEntry[mqd].openCount>0){
        qcbTblEntry[mqd].openCount--;
    }
    else if(qcbTblEntry[mqd].openCount <= 0){ 
        qcbTblEntry[mqd].bUsed=FALSE;
        memset(qcbTblEntry[mqd].name,0,sizeof(PMQ_NANE_LEN_MAX));
        qcbTblEntry[mqd].pQcb=NULL;
    }
    else{
        return FAILED;
    }
    return SUCCESS;
}

int pmq_send(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int msg_prio)
{

    /*접근하고자 하는 우체통 qcb를 가져온다.*/
    
    if(qcbTblEntry[mqd].bUsed==FALSE) return FAILED;
    Qcb* qcb = qcbTblEntry[mqd].pQcb;
    
    /*메시지 초기화 */
    Message * message = malloc(sizeof(Message));
    strcpy(message->data,msg_ptr);//메시지 복사!!!!

    message->size = msg_len;//메시지 길이
    message->priority = msg_prio;//메시지 우선순위
    
    /*우선순위에 따라 Message 리스트에 넣는다.*/
    if(qcb->msgCount==0){//아무것도 없었다면 head == tail
    
        
        qcb->pMsgHead = message;
        qcb->pMsgTail = message;
        qcb->msgCount++;
    }
    else{//tail에 넣는다. 단, 우선순위 고려
        BOOL flag = FALSE;
        Message * pPrev;

        for(Message *cursor = qcb->pMsgHead ; cursor!=NULL; ){
            if(cursor->priority < msg_prio){//자신보다 낮은 우선순위라면 삽입한다.
                
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
            
            }
            
            pPrev = cursor;
            cursor = cursor->pNext;
        }
        if(!flag){//NULL이 될 때 까지 자신보다 낮은 우선순위를 발견하지 못함.
            
            if(qcb->pMsgTail!=NULL){
                qcb->pMsgTail->pNext = message;
                message->pPrev = qcb->pMsgTail;
                
            }
            qcb->pMsgTail = message;
            pPrev->pNext = message;
            message->pPrev = pPrev;
            message->pNext = NULL;
        }

        qcb->msgCount++;
    }
    
    //Message를 집어넣었는데
    //Thread 리스트에서 대기 쓰레드를 가져온다.
    
    if(qcb!=NULL && qcb->waitThreadCount > 0){//대기하고 있는 스레드가 있다면
    
        //찾은 thread를 레디큐의 tail로 보낸다.

        Thread * qcbThread = qcb->pWaitQHead;
        

        if(qcb->waitThreadCount==1){//Head하나만 있었다는 뜻


            WaitingQueue_To_ReadyQueue(qcbThread);//Count가 0이상이면 널일리가 없음
    
            qcb->pWaitQHead=NULL;
            qcb->pWaitQTail=NULL;//qcb에 이제 남아있는 waitThread없음
            qcbThread->phNext=NULL;
            qcbThread->phPrev=NULL;
        }
        else{//여러개 있었음.
            
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
    
    }

    
    return SUCCESS;

}
ssize_t pmq_receive(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int* msg_prio)
{
    //무조건 헤드에서 먼저 받는다.
    
    /*접근하고자 하는 우체통 qcb를 가져온다.*/
    if(qcbTblEntry[mqd].bUsed==FALSE)
        return FAILED;
    Qcb* qcb = qcbTblEntry[mqd].pQcb;


    if(qcb->msgCount==0){/*Message queue에 가져올 Message가 없음.*/
        /* Waiting queue에 넣는다.*/


        if(qcb->waitThreadCount==0){

            qcb->pWaitQHead = pCurrentThread;
            qcb->pWaitQTail = pCurrentThread;
            InsertThreadToWaitingQueue(pCurrentThread);
        }
        else{
            qcb->pWaitQTail->phNext = pCurrentThread;
            pCurrentThread->phPrev = qcb->pWaitQTail;
            qcb->pWaitQTail = pCurrentThread;
            pCurrentThread->status = THREAD_STATUS_WAIT;
            InsertThreadToWaitingQueue(pCurrentThread);
        }
        qcb->waitThreadCount++;

        thread_t tid = thread_self();
        pCurrentThread=NULL;

        kill(mainPid,SIGALRM);
        kill(getpid(),SIGSTOP);

    }

        /*Message가져오기*/
        Message * message;
        
        if(qcb->msgCount>0){
            message = qcb->pMsgHead; //헤드에서 메시지 가져오기            
            qcb->msgCount--;
            qcb->pMsgHead = message->pNext; //다음 메시지를 헤드로 만들기
            //qcb->pMsgHead -> pPrev = NULL; //Head 삭제로 인한 prev NULL처리    TODO: 고쳐도 되나?
            message->pNext= NULL; //받아온 메시지에 연결된 next 연결고리도 끊어준다.
            
        }
        else{
            
            if(qcb->msgCount==0) return pmq_receive(mqd, msg_ptr, msg_len, msg_prio);
            //가져올 메시지가 없다는 뜻인듯.
        }
        
        if(message!=NULL){
            int len = (message->size > msg_len) ? msg_len : message->size;
            strncpy(msg_ptr,message->data, len);
            *msg_prio = message->priority;//priority 
            
            return len;//짧은 것의 길이 (읽은 만큼 반환하기)
        }
        else{
            return PMQ_NANE_LEN_MAX;
        }
}
