#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "Thread.h"

//TODO: 임시로 여기다가 선언 넣어줍미당
void * AppTask(void* param);
//-----------------------------

int	RunScheduler( void );
void	__ContextSwitch(int curpid, int newpid);

/* Running 상태의 Thread를 가리키는 변수 */
Thread* pCurrentThead;

#endif
