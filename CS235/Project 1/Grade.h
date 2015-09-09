//#pragma once

#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;

class Grade {

  private:
  string course_id;
  string stud_id;
  string grade;

  public:
  Grade(string course_id, string stud_id, string grade){}

  string getCourse_id() const { return course_id; }
  string getStud_id() const { return stud_id; }
  string getGrade() const { return grade; }

  void setCourse_id(string course_id){
    this->course_id = course_id;
  }
  void setStud_id(string stud_id){
    this->stud_id = stud_id;
  }
  void setGrade(string grade){
    this->grade = grade;
  }

  string toString() const {
    stringstream out;
    out << stud_id << "    " << grade << "    " << course_id;
    return out.str();
  };

  bool operator < (Grade g) const {
    return stud_id < g.stud_id ||
      (stud_id == g.stud_id && course_id < g.course_id) ||
      (stud_id == g.stud_id && course_id == g.course_id && grade < g.grade);
  }


};