#define _GNU_SOURCE
#include <sched.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"


int thread_create(thread_t *bthread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg)
{
    thread_t pid = clone(); //thread_t ??
    kill(pid,SIGSTOP);
    //Allocate TCB;
    //TCB initialize
    //branch..

}

int thread_suspend(thread_t tid)
{
    //move TCB -> waiting queue
    //tid.status = waiting;

    //if success return 0;
    //else (failed) return -1;
}

int thread_cancel(thread_t tid)
{
    //there is no self kill
    kill(tid,SIGKILL);
    //  from ready or waiting queue~
    //  remove TCB
    //  Deallocate tid's thread TCB
    return;
}

int thread_resume(thread_t tid)
{
    // we must to follow PPT's logic


    //if success return 0;
    //else (failed) return -1;
}

thread_t thread_self()
{

}
