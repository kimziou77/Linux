#ifndef __MYSHM_H__
#define __MYSHM_H__

#define SHM_MAX_SIZE (sizeof(struct __Calc) * 2)
#define PERMS 0644

struct __Calc {
    int x;
    int y;
    char op;
};
typedef struct __Calc Calc;

#endif // !__MYSHM_H__
