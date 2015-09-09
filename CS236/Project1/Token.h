#pragma once

#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;

class Token {
  private:
  string type;
  string value;
  int line;

  public:
  Token(string type, string value, int line){
    setType(type);
    setValue(value);
    setLine(line);
  }

  string getType() const { return type; }
  string getValue() const { return value; }
  int getLine() const { return line; }

  void setType(string type){
    this->type = type;
  }
  void setValue(string value){
    this->value = value;
  }
  void setLine(int line){
    this->line = line;
  }

  string toString() const {
    stringstream out;
    out << "(" << type << ",\"" << value << "\"," << line << ")";
    return out.str();
  }
};