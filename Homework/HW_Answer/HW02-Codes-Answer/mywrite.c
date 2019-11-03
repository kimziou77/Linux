/*
 * Student ID: 2017726001
 * Name: 홍길동
 */
#include "MyStudent.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PERMS 0644

int main(int argc, char const *argv[]) {
    int fd = 0;
    char *pathName = "./student.dat";
    ssize_t wSize = 0;

    char id[MAX_ID_SIZE + 1] = {
        '\0',
    };
    char name[MAX_NAME_SIZE + 1] = {
        '\0',
    };
    int score = 0;

    // 학생의 정보를 담기 위한 구조체 생성 및 초기화
    Student *student = (Student *)malloc(sizeof(Student));
    memset(student->id, '\0', MAX_ID_SIZE + 1);
    memset(student->name, '\0', MAX_NAME_SIZE + 1);
    student->score = 0;

    // 사용자 정보를 입력
    printf("ID: ");
    scanf("%s", id);
    fflush(stdin);

    printf("Name: ");
    scanf("%s", name);
    fflush(stdin);

    printf("Score: ");
    scanf("%d", &score);
    fflush(stdin);

    // 입력한 학생의 정보를 구조체에 복사
    strcpy(student->id, id);
    strcpy(student->name, name);
    student->score = score;

    // 학생의 정보를 담기 위한 파일 생성 또는 열기
    fd = open(pathName, O_CREAT | O_RDWR | O_APPEND, PERMS);
    if (fd == -1) {
        perror("open() error!");
        exit(-1);
    }

    // 학생의 정보를 파일에 쓰기
    wSize = write(fd, (Student *)student, sizeof(Student));
    if (wSize == -1) {
        perror("write() error!");
        exit(-2);
    }
    printf("Saved at %s.\n", pathName);

    free(student);
    close(fd);

    return 0;
}
