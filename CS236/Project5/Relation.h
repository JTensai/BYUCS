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
    stringstream ss;
    if (name == ""){
      for (string s : scheme.getAttrNames()){
        ss << s;
      }
      return ss.str();
    }
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

  void select_helper(Parameter& param, int& pos, bool& selected, Relation& r, vector<int>& columns, bool& must_compare){
    if (param.getType() == "STRING"){
      must_compare = true;
      for (Tuple t : getTuples()){
        int i = 0;
        for (string s : t.getValues()){
          if (s == param.getValue() && i == pos){
            selected = true;
            r.addTuple(t);
          }
          i++;
        }
      }
    }
    else{
      columns.push_back(pos);
    }
  }

  Relation select(Predicate& p){
    Relation r = Relation(p.getIdent());
    r.setScheme(getScheme());
    bool selected = false;
    bool must_compare = false;
    int pos = 0;
    vector<int> columns;
    vector<string> new_scheme;
    for (Parameter param : p.getParams()){
      select_helper(param, pos, selected, r, columns, must_compare);
      new_scheme.push_back(param.getValue());
      pos++;
    }
    if (selected){
      r.setScheme(new_scheme);
    } 
    else if (!must_compare){
      r.setTuples(getTuples());
    }
    return r;    
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

  Relation project(string name, vector<string> columns){
    stringstream ss;
    for (string s : columns){
      ss << s;
    }
    Relation r = Relation(name);
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

  // void rename(){}

  void myUnion(Relation& r){
    for (Tuple t : r.getTuples()){
      // cout << "Added tuple through union: " << t.toString() << endl;
      addTuple(t);
    }
  }

  Scheme joinScheme(vector<string> attr_names1, vector<string> attr_names2){
    vector<string> new_attr_names = attr_names1;
    map<int, int> duplicates;
    int j = attr_names1.size();
    for (string s : attr_names2){
      int i = 0;
      bool exists = false;
      for (string s2 : attr_names1){
        if (s == s2){
          duplicates[i] = j;
          exists = true;
        }
        i++;
      }
      if (!exists){
        new_attr_names.push_back(s);
      }
      j++;
    }
    Scheme new_scheme = Scheme(new_attr_names);
    new_scheme.setDuplicates(duplicates);
    return new_scheme;
  }

  void joinHelper(Scheme& new_scheme, Tuple& t, bool& add, vector<string>& values){
    for (int i = 0; i < t.getValues().size(); i++){
      bool valid = true;
      for (auto& entry : new_scheme.getDuplicates()){
        if (t.getValues().at(entry.first) != t.getValues().at(entry.second)){
          add = false;
          return;
        }
        if (i == entry.second){
          valid = false;
          break;
        }
      }
      if (valid)
        values.push_back(t.getValues().at(i));
    }
  }

  Relation join(Relation& r1, Relation& r2){    
    Scheme new_scheme = joinScheme(r1.getScheme().getAttrNames(), r2.getScheme().getAttrNames());

    Relation joined = Relation();
    joined.setScheme(new_scheme);
    joined.setName(r1.getName()+r2.getName());

    for (Tuple t1 : r1.getTuples()){
      for (Tuple t2 : r2.getTuples()){
        Tuple t = Tuple(t1,t2);
        // cout << t.toString() << endl;
        vector<string> values;
        bool add = true;
        joinHelper(new_scheme, t, add, values);
        if (add){
          joined.addTuple(values);
        }
      }
    }
    return joined;
  }

  string toStringHead(){
    bool first = true;
    stringstream ss;
    ss << getName() << "(";
    for (string s : getScheme().getAttrNames()){
      if (first){
        ss << s;
        first = false;
      }
      else{
        ss << "," << s;
      }
    } 
    ss << ")" << "? ";
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
        i++;
        count++;
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
      
      if (count > 0){
        ss << endl;
      }
    }
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