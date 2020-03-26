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

int main(int argc, char const *argv[]) {
    int fd = 0;
    char *pathName = "./student.dat";
    ssize_t rSize = 0;

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

    // 학생의 정보를 담고 있는 파일 열기
    fd = open(pathName, O_RDONLY);
    if (fd == -1) {
        perror("open() error!");
        exit(-1);
    }

    // 저장된 학생의 정보를 모두 읽고 화면에 출력
    do {
        rSize = read(fd, (Student *)student, sizeof(Student));

        if (rSize == sizeof(Student)) {
            // 읽은 정보가 존재하는 경우에, 화면에 출력
            printf("ID: %s, Name: %s, Score: %d\n", student->id, student->name,
                   student->score);

            memset(student->id, '\0', MAX_ID_SIZE + 1);
            memset(student->name, '\0', MAX_NAME_SIZE + 1);
            student->score = 0;
        } else if (rSize == -1) {
            // 오류가 발생한 경우에, 오류를 화면에 출력하고 종료
            perror("read() error!");
            exit(-2);
        }
    } while (rSize > 0);

    free(student);
    close(fd);

    return 0;
}
