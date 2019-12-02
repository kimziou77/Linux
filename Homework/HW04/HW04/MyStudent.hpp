#ifndef __MYSTUDENT_H__
#define __MYSTUDENT_H__

#include <string>

#define MAX_NAME_LEN 32

class Student {
  public:
    Student();
    Student(int id, std::string name, double score);

    void setId(int id);
    void setName(std::string name);
    void setScore(double score);

    int getId(void);
    std::string getName(void);
    double getScore(void);

  private:
    int id;
    char name[MAX_NAME_LEN + 1];
    double score;
};

#endif
