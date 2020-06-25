#include "MsgQueue.h"

pmqd_t pmq_open(const char* name, int flags, mode_t perm, pmq_attr* attr)
{

}

int pmq_close(pmqd_t mqd)
{

}
int pmq_send(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int msg_prio)
{

}
ssize_t pmq_receive(pmqd_t mqd, char* msg_ptr, size_t msg_len, unsigned int* msg_prio)
{
    
}
