#include "MyStudent.hpp"
#include <string.h>
#include <string>

Student::Student() {
    this->id = -1;
    memset(this->name, 0x00, MAX_NAME_LEN + 1);
    this->score = -1.0;
}

Student::Student(int id, std::string name, double score) {
    this->id = id;
    memcpy(this->name, name.c_str(), MAX_NAME_LEN);
    this->score = score;
}

void Student::setId(int id) { this->id = id; }

void Student::setName(std::string name) {
    memcpy(this->name, name.c_str(), MAX_NAME_LEN);
}

void Student::setScore(double score) { this->score = score; }

int Student::getId(void) { return this->id; }

std::string Student::getName(void) { return std::string(this->name); }

double Student::getScore(void) { return this->score; }
