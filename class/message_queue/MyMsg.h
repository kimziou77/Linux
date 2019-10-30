#ifndef __MYMSG_H__
#define __MYMSH_H__

#define MSG_TYPE_CALC 1 // 숫자 및 연산자
#define MSG_TYPE_RSLT 2 // 결과 메시지

#define MSG_SIZE_CALC (sizeof(MsgCalc)- sizeof(long))
#define MSG_SIZE_RSLT (sizeof(MsgRslt)- sizeof(long))

struct __Calc{
    int x;
    int y;
    char op;
};
typedef struct __Calc Calc;

struct __MsgCalc{
    long mtype;
    struct __Calc calc; //Calc calc;??
};
typedef struct __MsgCalc MsgCalc;

struct __MsgRslt{
    long mtype;
    int result;
};
typedef struct __MsgRslt MsgRslt;

#endif
