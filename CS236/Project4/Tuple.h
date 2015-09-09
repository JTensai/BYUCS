#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector> 

using std::vector;
using std::string;
using std::stringstream;

class Tuple : public vector<string>{
  private:
  vector<string> values;

  public:
  Tuple(){}

  Tuple(vector<string> values){
    setValues(values);
  }

  Tuple(Tuple& t1, Tuple& t2){
    vector<string> new_values = t1.getValues();
    for (string s : t2.getValues()){
      new_values.push_back(s);
    }
    setValues(new_values);
  }

  void setValues(vector<string> values){
    this->values = values;
  }

  vector<string> getValues() const{
    return values;
  }



  string toString(){
    stringstream ss;
    for (string s : values){
      ss << s << "    ";
    }
    ss << "\t";
    return ss.str();
  }

  bool operator< (const Tuple& t) const{
    return getValues() < t.getValues();
  }

  // bool operator< (const Tuple& t) const{
  //   list<string> original = getValues();
  //   list<string> other = t.getValues();

  //   list<string>::iterator it1 = original.begin();
  //   list<string>::iterator it2 = other.begin();
  //   while (it1 != original.end()){
  //     if (*it1 < *it2){
  //       return true;
  //     }
  //     else if (*it1 == *it2){
  //       it1++;
  //       it2++;
  //     }
  //     else {
  //       return false;
  //     }
  //   }
  // }

};