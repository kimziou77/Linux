#ifndef _HEADERS_H_
#define _HEADERS_H_
#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include "ReadyQueue.h"

enum{FALSE=0,TRUE=1};
void * AppTask(void* param);
void Tastcase(void);
#endif
