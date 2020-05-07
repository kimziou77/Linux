#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "Thread.h"

int	RunScheduler( void );
void	__ContextSwitch(int curpid, int newpid);

Thread* pCurrentThread;

#endif

