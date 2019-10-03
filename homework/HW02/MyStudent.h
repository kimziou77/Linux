/*
 * Last updated by hwyncho on 2019. 09. 04
 */
#ifndef __MYSTUDENT_H__
#define __MYSTUDENT_H__

#define MAX_ID_SIZE 16
#define MAX_NAME_SIZE 32

struct _Student {
    char id[MAX_ID_SIZE + 1];
    char name[MAX_NAME_SIZE + 1];
    int score;
};
typedef struct _Student Student;

#endif // !__MYSTUDENT_H__
