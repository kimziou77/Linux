#ifndef _HEADERS_H_
#define _HEADERS_H_
#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include "ReadyQueue.h"
#include <stdio.h>
#include <stdlib.h>

enum{FALSE = 0, TRUE = 1};
enum{FAILED = -1, SUCCESS = 0};

void * AppTask(void* param);
void signalHandler(int signum);
void Tastcase(void);
int find_tid(int pid);

void print_pWaitingQueue();
void print_pReadyQueue();
void print_pThreadEnt();
void print_pCurrentThread();
int FindEmptyThreadTable();
#endif
