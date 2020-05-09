#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "Thread.h"

int	RunScheduler( void );
void	__ContextSwitch(int curpid, int newpid);

/* Running 상태의 Thread를 가리키는 변수 */
Thread* pCurrentThread;

#endif
