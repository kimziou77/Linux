#ifndef TEST_CASE_3_H
#define TEST_CASE_3_H

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "Thread.h"
#include "MsgQueue.h"

void* Tc3ThreadPing(void* param);
void* Tc3ThreadPong(void* param);
void TestCase3(void);

#endif
