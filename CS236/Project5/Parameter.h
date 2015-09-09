#pragma once

#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;

class Parameter {
  private:
  string type;
  string value;

  public:
  Parameter(string type, string value){
    setType(type);
    setValue(value);
  }

  string getType() const { return type; }
  string getValue() const { return value; }

  void setType(string type){
    this->type = type;
  }
  void setValue(string value){
    this->value = value;
  }

  string toString() const {
    stringstream out;
    out << "(" << type << ",\"" << value << "\")";
    return out.str();
  }
};