#include "Headers.h"

void InsertThreadToReadyQueue(Thread *pNewThread, int priority){
    //Thread -> Ready

}
Thread *GetThreadByNum(int objnum){
    //objNum으로 부터
    //ReadyQueue에서 위치를 알아온다. 포인터만 알아오면 된다.
}
Thread *GetThreadFromObjFreeList(){
    //Waiting Queue의 head로부터 Thread를 가지고 온다.
}
BOOL DeleteThreadFromReadyQueue(Thread *pObj){
    //Waiting Queue로부터 Thread를 Delete 해준다.
}
void WaitingQueue_To_ReadyQueue(Thread *pObj){
    //Waiting-> Ready
    //FreeList의 head로 넣는다.???
    //ReadyQueue의 head로 넣는다.
    //내개ㅏ 해야할건 Tail로 넣는것일텐데
}

//wating queue의 tail로 넣는 함수

//해당 스레드를 waiting queue에서 빼네는 함수.
