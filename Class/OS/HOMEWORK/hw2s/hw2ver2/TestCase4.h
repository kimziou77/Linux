#ifndef TEST_CASE_4_H
#define TEST_CASE_4_H

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include "Thread.h"

#define TOTAL_THREAD_NUM (6)
void* Tc4ThreadProc(void* param);
void TestCase4(void);

#endif
