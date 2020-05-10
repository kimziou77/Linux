#ifndef TEST_CASE_3_H
#define TEST_CASE_3_H

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include "Thread.h"

#define TOTAL_THREAD_NUM (9)
void* Tc3ThreadProc(void* param);
void TestCase3(void);

#endif
