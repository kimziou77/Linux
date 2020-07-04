#ifndef TEST_CASE_2_H
#define TEST_CASE_2_H

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "Thread.h"
#include "MsgQueue.h"

void* Tc2ThreadSender(void* param);
void* Tc2ThreadReceiver(void* param);
void TestCase2(void);

#endif
