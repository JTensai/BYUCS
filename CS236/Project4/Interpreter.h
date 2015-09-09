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

  Relation joinIntermediate(vector<Relation>& inter){
    Relation joined = Relation();
    bool first = true;
    for(Relation r : inter){
      if (first){
        joined = r;
        first = false;
      }
      else{
        joined = joined.join(joined, r);
      }
    }
    return joined;
  }

  string evaluateRulesStart(){
    stringstream ss;
    int num_of_passes = 0;
    int previous_count = db.getCount();

    while (true){
      previous_count = db.getCount();
      evaluateRulesRec(num_of_passes);
      if (previous_count == db.getCount()){
        break;
      }
    }
    ss << "Converged after " << num_of_passes << " passes through the Rules." << endl;
    return ss.str();
  }

  void evaluateRulesRec(int& num_of_passes){
    num_of_passes++;
    for (Rule r : dp.getRules()){
      vector<Relation> intermediate;
      Relation selected;
      for (Predicate p : r.getPredList()){
        Relation temp = Relation(p.getIdent());
        temp.setScheme(p.getParamValues());

        for (Tuple t : db.getRelations()[p.getIdent()].getTuples()){
          temp.addTuple(t.getValues());
        }

        selected = temp.select(p);
        intermediate.push_back(selected);
      }
      if (r.getPredList().size() == 1){
        Relation projected = selected.project(r.getPredicate().getIdent(), r.getPredicate().getParamValues());
        db.addRelation(projected);
      }
      else{
        Relation joined = joinIntermediate(intermediate);
        Relation projected = joined.project(r.getPredicate().getIdent(), r.getPredicate().getParamValues());
        db.addRelation(projected);
      }
    }
  }

  string evaluateQueries(){
    stringstream ss;
    for (Predicate p : dp.getQueries()){
      // cout << p.toString() << endl;
      ss << db.selects(p);
    }
    return ss.str();
  }

  string toString(){
    return db.toString();
  }
};