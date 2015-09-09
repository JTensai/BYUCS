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
  map<int, int> duplicates;

  public:
  Scheme(){}

  Scheme(vector<string> values){
    setAttrNames(values);
  }

  void setAttrNames(vector<string> values){
    attr_names = values;
  }
  void setDuplicates(map<int, int> values){
    duplicates = values;
  }

  vector<string> getAttrNames(){
    return attr_names;
  }
  map<int, int> getDuplicates(){
    return duplicates;
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