#include "MyStudent.hpp"
#include <fcntl.h>
#include <iostream>
#include <list>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define PERMS 0644
using namespace std;

int main(void) {

    /* 아래에 코드를 작성하세요. */
    list<Student> stuList;

    while (1) {
        string input;
        cout << "ID (input \`q\` to terminate) : ";
        cin >> input;

        if (input.compare("q") == 0) {
            cout << ">> Terminate input." << endl;
            break;
        }

        int id = stoi(input);
        string name = "";
        cout << "<< Name : ";
        cin >> name;

        double score = -1.0;
        cout << "<< Score : ";
        cin >> score;

        Student stu(id, name, score);
        stuList.push_back(stu);
        cout << ">> Successfully added to list!" << endl;
    }

    string filepath = "./StudentList.dat";
    int fd = open(filepath.c_str(), O_CREAT | O_APPEND | O_WRONLY, PERMS);
    if (fd == -1) {
        perror("open() error");
        return 1;
    }
    cout << "fd  : " << fd << endl;
    list<Student>::iterator iter;
    for (iter = stuList.begin(); iter != stuList.end(); ++iter) {
        if (write(fd, &(*iter), sizeof(Student) == -1)) {
            perror("write() error");
            return 2;
        }
    }

    cout << ">> " << stuList.size()
         << " students' info was successfully saved to the" << filepath << endl;

    char *pathName = "./StudentList.dat";
    int fd2 = open(pathName, O_RDONLY, PERMS);
    if (fd2 == -1) {
        perror("open() error!");
        exit(-1);
    }
    printf("fd2 : %d\n", fd2);
    int rSize = 0;
    list<Student> stuList2;
    list<Student>::iterator iter2;
    Student *st = new Student;
    st->setId(0);
    st->setName("");
    st->setScore(0);
    do {

        rSize = read(fd, (Student *)st, sizeof(Student));
        printf("rSize : %d\n", rSize);
        if (rSize == -1) {
            perror("read() error");
            return 2;
        }
        stuList2.push_back(*st);
    } while (rSize > 0);

    printf("list size : %d\n", stuList.size());
    int num = 1;
    for (iter2 = stuList2.begin(); iter2 != stuList2.end(); ++iter2) {
        int id = iter2->getId();
        std::string name = iter2->getName();
        double score = iter2->getScore();

        // Student st(id, name, score);
        // stuList.push_back(st);
        cout << num << "번쨰 학생 정보" << endl;
        cout << "ID : " << id << ", Name : " << name << ", Score : " << score
             << endl;
    }
    close(fd);
    close(fd2);
    delete st;
    return 0;
}

/*
    stuList.clear();

    // failed loading file

    char *pathName = "./StudentList.dat";
    fd = open(pathName, O_RDONLY);
    if (fd == -1) {
        perror("open() error!");
        exit(-1);
    }

    size_t rSize = 0;
    do {
        Student *st = new Student();
        rSize = read(fd, (Student *)st, sizeof(Student));
        if (rSize == -1) {
            perror("read() error");
            return 2;
        }
        stuList.push_back(*st);
    } while (rSize > 0);

    // printf("list size : %d\n", stuList.size());
    int num = 1;
    for (iter = stuList.begin(); iter != stuList.end(); ++iter) {
        int id = iter->getId();
        std::string name = iter->getName();
        double score = iter->getScore();

        Student st(id, name, score);
        // stuList.push_back(st);
        cout << num << "번쨰 학생 정보" << endl;
        cout << "ID : " << id << ", Name : " << name << ", Score : " << score
             << endl;
    }
*/
