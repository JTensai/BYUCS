#include <iostream>
#include <sstream>
#include <string>
#include "DatalogProgram.h"
#include "Database.h"
#include "Relation.h"

using std::string;
using std::stringstream;

class Interpreter {
  private:
    DatalogProgram dp;
    Database db;

  public:
  Interpreter(char* source){
    dp = DatalogProgram(source);
    db = Database();
  }

  void createSchemes(){
    for (Predicate p : dp.getSchemes()){
      db.createRelation(p.getIdent(), p.getParamValues());
    }
  }

  void createTuples(){
    for (Predicate p : dp.getFacts()){
      db.addTupleToRelation(p.getIdent(), p.getParamValues());
    }
  }

  string evaluateQueries(){
    stringstream ss;
    for (Predicate p : dp.getQueries()){
      ss << db.select(p);
    }
    return ss.str();
  }



  // // project sc from csg
  // void testProject(){
  //   list<string> columns;
  //   columns.push_back("S");
  //   columns.push_back("C");

  //   db.testProject("csg", columns);
  // }


  string toString(){
    return db.toString();
  }
};