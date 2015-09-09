#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include "Scheme.h"
#include "Tuple.h"

using std::vector;
using std::endl;
using std::set;
using std::string;
using std::stringstream;

class Relation {
  private:
    string name;
    Scheme scheme;
    set<Tuple> tuples;

  public:
  Relation(){}

  Relation(string name){
    setName(name);
  }

  void setName(string n){
    name = n;
  }
  string getName(){
    return name;
  }

  void setScheme(Scheme s){
    scheme = s;
  }
  void setScheme(vector<string> values){
    setScheme(Scheme(values));
  }
  Scheme getScheme(){
    return scheme;
  }

  void setTuples(set<Tuple> t){
    tuples = t;
  }
  void addTuple(Tuple t){
    tuples.insert(t);
  }
  void addTuple(vector<string> values){
    tuples.insert(Tuple(values));
  }
  set<Tuple> getTuples(){
    return tuples;
  }

  Relation select(int pos, string value){
    Relation r = Relation(getName());

    r.setScheme(getScheme());
    for (Tuple t : tuples){
      int i = 0;
      for (string s : t.getValues()){
        if (s == value && i == pos){
          r.addTuple(t);
        }
        i++;
      }
    }
    return r;
  }

  Relation project(vector<string> columns){
    stringstream ss;
    for (string s : columns){
      ss << s;
    }
    Relation r = Relation(ss.str());
    r.setScheme(columns);

    vector<int> col_order;
    int i = 0;
    for (string s : columns){
      for (string s2 : scheme.getAttrNames()){
        if (s == s2){
          col_order.push_back(i);
          break;
        }
        i++;
      }
      i = 0;
    }

    for (Tuple tuple : tuples){
      Tuple t;
      vector<string> new_values;
      vector<string> old_values;
      for (string s : tuple.getValues()){
        old_values.push_back(s);
      }
      for (int i : col_order){
        new_values.push_back(old_values[i]);
      }
      t.setValues(new_values);
      r.addTuple(t);
    }

    return r;
  }

  void rename(){}

  string toStringHead(){
    stringstream ss;
    ss << getName() << "? ";
    if (getTuples().size() > 0){
      ss << "Yes(" << getTuples().size() << ")" << endl;
    }
    else {
      ss << "No" << endl;
    }
    return ss.str();
  }

  string toStringBody(Tuple t, vector<int> columns, int& count){
    stringstream ss;
    bool first = true;
    for (int col : columns){
      if (getTuples().size() > 0 && first){
        ss << "  ";
      }
      int i = 0;
      for (string s : t.getValues()){
        if (i == col){
          if (getScheme().getAttrNames().size() > 0){
            if (first){
              ss << getScheme().getAttrNames()[col] << "=" << s;
              first = false;
            }
            else
              ss << ", " << getScheme().getAttrNames()[col] << "=" << s;
          }
        }
        count++;
        i++;
      }
    }
    return ss.str();
  }

  string toStringQuery(vector<int> columns){
    stringstream ss;
    ss << toStringHead();
    int count = 0;
    for (Tuple t : getTuples()){
      ss << toStringBody(t, columns, count);
      
      if (count > 1){
        ss << endl;
      }
    }
    // for ()
    // SK(A,'c')? Yes(2)
    //   A='a'
    //   A='b'
    // SK('b','c')? Yes(1)
    // SK('c','c')? No
    // SK(A,B)? Yes(3)
    //   A='a', B='c'
    //   A='b', B='b'
    //   A='b', B='c'

    return ss.str();
  }

  string toString(){
    stringstream ss;
    ss << "__________________________________________" << endl;
    ss << "|                                        |" << endl;
    ss << "|               " << getName() << "            (" << getTuples().size() << ") \t |"<< endl;
    ss << "|                                        |" << endl;
    ss << "|----------------------------------------|" << endl;
    ss << "\t" << getScheme().toString() << endl;    
    ss << "|----------------------------------------|" << endl;
    for (Tuple t : getTuples()){
      ss << "\t" << t.toString() << endl;
    }
    ss << "|________________________________________|" << endl;
    return ss.str();
    // return "relation found";
  }
};