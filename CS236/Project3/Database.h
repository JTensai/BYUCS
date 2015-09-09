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

  void addTupleToRelation(string name, vector<string> values){
    Relation r = relations[name];
    r.addTuple(values);
    relations[r.getName()] = r;
  }

  string select(Predicate p){
    // find the correct relation to select from
    Relation r = relations[p.getIdent()];
    r.setName(p.toString());

    int pos = 0;
    vector<int> columns;
    vector<string> new_scheme;

    for (Parameter param : p.getParams()){
      bool new_value = true;
      for (string s : new_scheme){
        if (s == param.getValue()){
          new_value = false;
        }
      }

      if (new_value){
        if (param.getType() == "STRING"){
          r = r.select(pos, param.getValue());
        }
        else{
          columns.push_back(pos);
        }

      // cout << "made it through the new_scheme loop" << endl;
        new_scheme.push_back(param.getValue());
        pos++;
      }
    }
    r.setScheme(new_scheme);

    return r.toStringQuery(columns);
  }

  void project(string relation, vector<string> columns){
    Relation r = relations[relation];
    r = r.project(columns);
    relations[r.getName()] = r;
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