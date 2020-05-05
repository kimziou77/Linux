#ifndef __THREAD_H__
#define __THREAD_H__
#define _GNU_SOURCE
#define TIMESLICE		(1)
#define MAX_READYQUEUE_NUM	(8)		/* Ready queue의 개수 */
#define MAX_THREAD_NUM		(64)	/* 생성 가능한 최대 Thread의 개수 */

#include <sched.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

typedef int	BOOL;
typedef int	thread_t;
typedef struct _Thread Thread;
typedef void	thread_attr_t;

typedef enum{
	THREAD_STATUS_RUN = 0,
	THREAD_STATUS_READY = 1,
	THREAD_STATUS_WAIT = 2,
	THREAD_STATUS_ZOMBIE = 3,
} ThreadStatus;

typedef struct _Thread{ //TCB
//스레드 정보를 저장하는 구조체, 스케줄링에 필요한 정보를 저장한다.
//TODO: 스레드 생성시 해당 스레드를 위한 TCB를 메모리에서 할당후 초기화 시켜준다.
	int		stackSize;//스레드 스택사이즈
	void*		stackAddr;//스택의 시작 주소
	ThreadStatus	status; //running(0)이아니면 ready(1) wait(2) 상태이어야 한다.
	int		exitCode;
	pid_t		pid; //스레드 아이디 tid?? clone으로 생성된 pid를 여기에 저장한다.
	int		priority;
	Thread*		phNext;//서로 인접한 스레드와 연결
	Thread*		phPrev;
} Thread;

typedef struct _ReadyQueueEnt {
	int	queueCount;
	Thread* pHead;
	Thread* pTail;
} ReadyQueueEnt;

typedef struct _ThreadTblEnt {
	BOOL	bUsed;   
	Thread* pThread;
} ThreadTblEnt;
ThreadTblEnt pThreadTbEnt[MAX_THREAD_NUM];
//ThreadTblEnt의 index번호를 스레드 아이디로 한다.

ReadyQueueEnt pReadyQueueEnt[MAX_READYQUEUE_NUM];
//얘는 왜 배열이야? -> 내부에 포인터로 되어있음

Thread* pWaitingQueueHead;
Thread* pWaitingQueueTail;


int 		thread_create(thread_t *thread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg);//clone함수 사용
//(생성된 thread_id 반환, NULL, 우선순위,스레드가 실행할 함수 이름, 실행함수인자)

//나머지 스레드들은 모두 정지해야하는데, signal을 사용할것임.//깨울때도
int 		thread_suspend(thread_t tid);
int 		thread_cancel(thread_t tid);
int		thread_resume(thread_t tid);
thread_t 		thread_self();

int thread_join(thread_t tid, void ** retval);
int thread_exit(void * retval);

#endif /* __THREAD_H__ */
