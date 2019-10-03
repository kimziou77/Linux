/*
 * Student ID: 2018203062
 * Name: Kimsubin
 */
#include "MyStudent.hpp"
#include <fcntl.h>
#include <iostream>
#include <list>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define PERMS 0644
using namespace std;

int main(void) {

    // use systemcall read file info to list and print info program
    // 1st student info
    // ID: 921205, Name : jin, Score : 99.3

    int fd = 0;
    char *pathName = "./StudentList.dat";
    size_t rSize = 5;
    list<Student> stuList;
    list<Student>::iterator iter;

    // Student *st = new Student;
    Student *st = (Student *)malloc(sizeof(Student));
    st->setId(0);
    st->setName("");
    st->setScore(0);

    fd = open(pathName, O_RDONLY);
    if (fd == -1) {
        perror("open() error!");
        exit(-1);
    }

    do {

        rSize = read(fd, (Student *)st, sizeof(Student));
        if (rSize == -1) {
            perror("read() error");
            return 2;
        }
        stuList.push_back(*st);

    } while (rSize > 0);

    int num = 1;

    for (iter = stuList.begin(); iter != stuList.end(); ++iter) {
        int id = iter->getId();
        std::string name = iter->getName();
        double score = iter->getScore();

        // Student st(id, name, score);
        cout << num++ << "번쨰 학생 정보" << endl;
        cout << "ID : " << id << ", Name : " << name << ", Score : " << score
             << endl;
    }

    close(fd);

    return 0;
}

/*
// input stuList <- file student
    int fd = 0;
    string pathName = "./StudentList.dat";

    fd = open(pathName.c_str, O_RDONLY, PERMS);
    if (fd == -1) {
        perror("open() error!");
        exit(-1);
    }

    Student *st = (Student *)malloc(sizeof(Student));
    size_t rSize = 0;

    do {
        rSize = read(fd, (Student *)st, sizeof(Student));

        if (rSize == -1) {
            perror("read() error");
            return 2;
        }

        stuList.push_back(*st);

    } while (rSize > 0);

    printf("list size : %d\n", stuList.size());

    for (iter = stuList.begin(); iter != stuList.end(); ++iter) {
        int id = iter->getId();
        string name = iter->getName();
        double score = iter->getScore();

        Student st(id, name, score);
        // stuList.push_back(st);
        printf("ID : %d, Name : %s , Score : %lf\n", id, name, score);
    }

    close(fd);

*/

/*

    // rSize = read(fd, (Student *)st, sizeof(Student));
    // if (rSize == -1) {
    //     perror("read() error!");
    //     exit(-2);
    // }
    // if (rSize == 0)
    //     break;
    // printf("fd : %d, rSize: %ld \n",fd,rSize);
    // open file
    // Student *st = new Student();
*/