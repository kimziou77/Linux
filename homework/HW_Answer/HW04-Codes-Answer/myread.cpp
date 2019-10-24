#include "MyStudent.hpp"
#include <fcntl.h>
#include <iostream>
#include <list>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main(void) {
    int fd = 0;
    string filepath = "./StudentList.dat";

    fd = open(filepath.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("open() error");
        return 1;
    }

    ssize_t rSize = 0;
    Student stu = Student();
    list<Student> stuList;

    do {
        rSize = read(fd, &stu, sizeof(Student));

        if (rSize == -1) {
            perror("read() error!");
            return 2;
        }

        if (rSize == 0) {
            break;
        }

        stuList.push_back(stu);
    } while (rSize > 0);

    close(fd);

    list<Student>::iterator iter;
    int i = 0;

    for (iter = stuList.begin(), i = 1; iter != stuList.end(); ++iter, ++i) {
        cout << i << " 번째 학생 정보" << endl;
        cout << " ID: " << iter->getId();
        cout << ", Name: " << iter->getName();
        cout << ", Score: " << iter->getScore();
        cout << endl;
    }

    return 0;
}
