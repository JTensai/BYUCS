//#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using std::string;
using std::stringstream;

class Result {

  private:
  string id;
  double gpa;
  string name;

  public:
  Result(string id, double gpa, string name){}

  string getID() const { return id; }
  double getGpa() const { return gpa; }
  string getName() const { return name; }

  void setID(string id){
    this->id = id;
  }
  void setGpa(double gpa){
    this->gpa = gpa;
  }
  void setName(string name){
    this->name = name;
  }

  string toString() const {
    stringstream out;
    out << id << "    " << std::fixed << std::setprecision (2) << std::setfill('0') << getGpa() << "    " << name;
    return out.str();
  };
};