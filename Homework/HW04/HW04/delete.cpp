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

int main(int argc, char const *argv[]) {
    string path_name = "./StudentList.dat";
    int fd = 0;
    fd = open(path_name.c_str, O_RDONLY);

    ssize_t rsize = 0;
    list<Student> stuList;
    Student *stu = new Student();
    string s;
    cout << "삭제할 학번을 입력하세요 : ";
    cin >> s;
    stat(path_name.c_str, (Student *)stu);

    delete stu;
    return 0;
}
