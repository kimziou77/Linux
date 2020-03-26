#ifndef __MYSHM_H__
#define __MYSHM_H__
#define SHM_MAX_SIZE 512
#define PERMS 0666
#define MY_SHM_KEY 1234
int creatShm(int key);
int openShm(int key);
int closeShm(int shmid);
#endif // !__MYSHM_H__
