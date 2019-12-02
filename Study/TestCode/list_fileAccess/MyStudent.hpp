#ifndef __MYSTUDENT_H__
#define __MYSTUDENT_H__

#include <string>
#define MAX_NAME_LEN 32
using namespace std;
class Student {
    int id;
    char name[MAX_NAME_LEN + 1];
    double score;

  public:
    Student();
    Student(int id, string name, double score);

    void setId(int id);
    void setName(string name);
    void setScore(double score);

    int getId(void);
    string getName(void);
    double getScore(void);
};

#endif
