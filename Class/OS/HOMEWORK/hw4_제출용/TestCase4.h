#ifndef TEST_CASE_4_H
#define TEST_CASE_4_H

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "Thread.h"
#include "MsgQueue.h"

void* Tc4ThreadPing(void* param);
void* Tc4ThreadPong(void* param);
void TestCase4(void);

#endif
