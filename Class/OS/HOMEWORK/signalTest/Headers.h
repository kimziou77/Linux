#ifndef _HEADERS_H_
#define _HEADERS_H_
#include "Thread.h"
#include "Scheduler.h"
#include "ReadyQueue.h"
#include <stdio.h>
#include <stdlib.h>
enum{FALSE = 0, TRUE = 1};
enum{FAILED = -1, SUCCESS = 0};
#define NON_EXIT -62
int mainPid;
void * AppTask(void* param);
void signalHandler(int signum);
void wakeUp(int signum);

void Tastcase(void);
// thread_t find_tid(int pid);
void schedule(int signum);
void print_pWaitingQueue();
void print_pReadyQueue();
void print_pThreadEnt();
void print_pCurrentThread();

int FindEmptyThreadTable();
#endif
