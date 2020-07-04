#ifndef TEST_CASE_1_H
#define TEST_CASE_1_H

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "Thread.h"
#include "MsgQueue.h"

void* Tc1ThreadSender(void* param);
void* Tc1ThreadReceiver(void* param);
void TestCase1(void);

#endif
