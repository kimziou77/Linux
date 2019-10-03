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
    int fd = open(filepath.c_str(), O_CREAT | O_APPEND | O_RDWR, PERMS);
    if (fd == -1) {
        perror("open() error");
        return 1;
    }
    // cout << "fd  : " << fd << endl;
    list<Student>::iterator iter;
    for (iter = stuList.begin(); iter != stuList.end(); ++iter) {
        int tmp = write(fd, (Student *)&(*iter), sizeof(Student));
        if (tmp == -1) {
            perror("write() error");
            return 2;
        }
    }
    close(fd);
    cout << ">> " << stuList.size()
         << " students' info was successfully saved to the" << filepath << endl;

    return 0;
}