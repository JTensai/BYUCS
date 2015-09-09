#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::stringstream;

class Scheme {
  private:
  vector<string> attr_names;

  public:
  Scheme(){}

  Scheme(vector<string> values){
    setAttrNames(values);
  }

  void setAttrNames(vector<string> values){
    attr_names = values;
  }

  vector<string> getAttrNames(){
    return attr_names;
  }


  string toString(){
    stringstream ss;
    for (string s : attr_names){
      ss << s << "\t    ";
    }
    ss << "\t";
    return ss.str();
  }
};