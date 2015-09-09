//#pragma once

#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;

class Student {

  private:

  string name;
  string id;
  string phone;
  string address;

  public:
  Student(string name, string id, string phone, string address){}

  string getName() const { return name; }
  string getID() const { return id; }
  string getPhone() const { return phone; }
  string getAddress() const { return address; }

  void setName(string name){
    this->name = name;
  }
  void setID(string id){
    this->id = id;
  }
  void setPhone(string phone){
    this->phone = phone;
  }
  void setAddress(string address){
    this->address = address;
  }

  string toString() const {
    stringstream out;
    out << name << 
    "\n" << id <<
    "\n" << phone <<
    "\n" << address;
    return out.str();
  };

  bool operator < (Student s) const {
    return id < s.id;
  }

};