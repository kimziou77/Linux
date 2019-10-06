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

    int fd = 0;
    char *pathName = "./StudentList.dat";
    size_t rSize = 5;
    list<Student> stuList;
    list<Student>::iterator iter;

    Student *st = new Student();
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
        if (rSize == 0)
            break;
        stuList.push_back(*st);

    } while (rSize > 0);

    int num = 1;

    for (iter = stuList.begin(); iter != stuList.end(); ++iter) {
        int id = iter->getId();
        std::string name = iter->getName();
        double score = iter->getScore();

        // Student st(id, name, score);
        cout << num++ << "번째 학생 정보" << endl;
        cout << "ID : " << id << ", Name : " << name << ", Score : " << score
             << endl;
    }

    close(fd);

    return 0;
}
