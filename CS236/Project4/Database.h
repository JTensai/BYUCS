#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "Relation.h"

using std::map;
using std::vector;
using std::string;
using std::stringstream;

class Database {
  private:
    map<string, Relation> relations;

  public:
  Database(){}

  map<string, Relation> getRelations(){
    return relations;
  }

  void createRelation(string name, vector<string> scheme){
    Relation r = Relation(name);
    r.setScheme(scheme);
    relations[r.getName()] = r;
  }

  void addRelation(Relation& r){
    if(relations.count(r.getName()) > 0){
      // cout << "relation found, doing union" << endl;
      relations[r.getName()].myUnion(r);
    }
    else{
      // cout << "new relation" << endl;
      relations[r.getName()] = r;
    }
    // cout << relations[r.getName()].toString() << endl;
  }

  void addTupleToRelation(string& name, vector<string>& values){
    relations[name].addTuple(values);
  }
  void addTupleToRelation(string name, Tuple t){
    relations[name].addTuple(t);
  }

  int getCount(){
    // returns count of total relations + total tuples
    int size = 0;
    for (auto& entry : relations){
      size += entry.second.getTuples().size();
      size++;
    }
    return size;
  }

  Relation select(Predicate p){
    // find the correct relation to select from
    Relation r = relations[p.getIdent()];
    r.setName(p.getIdent());

    int pos = 0;
    vector<int> columns;
    vector<string> new_scheme;

    for (Parameter param : p.getParams()){
      if (param.getType() == "STRING"){
        r = r.select(pos, param.getValue());
      }
      else{
        columns.push_back(pos);
      }
      new_scheme.push_back(param.getValue());
      pos++;
    }
    r.setScheme(new_scheme);
    return r;
  }

  string selects(Predicate p){
    // find the correct relation to select from
    Relation r = relations[p.getIdent()];
    r.setName(p.getIdent());

    int pos = 0;
    vector<int> columns;
    vector<string> new_scheme;

    for (Parameter param : p.getParams()){
      if (param.getType() == "STRING"){
        r = r.select(pos, param.getValue());
      }
      else{
        columns.push_back(pos);
      }
      new_scheme.push_back(param.getValue());
      pos++;
    }
    r.setScheme(new_scheme);
    return r.toStringQuery(columns);
  }


  string toString(){
    // return print_me.str();
    stringstream ss;
    for (auto& entry : relations) {
      ss << entry.second.toString() << endl;
    }
    return ss.str();
  }
}; 