#include <iostream>
#include <sstream>
#include <string>
#include "DatalogProgram.h"
#include "Database.h"
#include "Relation.h"
#include "Graph.h"

using std::string;
using std::stringstream;

class Interpreter {
  private:
    DatalogProgram dp;
    Database db;
    Graph graph;

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

  void buildGraph(){
    int i = 0;
    for (Rule r : dp.getRules()){
      // Builds graph of Rule Predicates (no dependencies)
      stringstream ss;
      ss << "R" << ++i;
      string ident = ss.str();
      graph.addNode(r, ident);
    }
    i = 0;
    for (Rule r : dp.getRules()){
      // adds dependencies to each Rule in graph
      stringstream ss;
      ss << "R" << ++i;
      string ident = ss.str();
      graph.buildRuleDependencies(r, ident);
    }
    i = 0;
    for (Predicate p : dp.getQueries()){
      // Builds graph of Queries (no dependencies)
      stringstream ss;
      ss << "Q" << ++i;
      string ident = ss.str();
      graph.addNode(p, ident);
    }
    i = 0;
    for (Predicate p : dp.getQueries()){
      // adds dependencies to each Query in graph
      stringstream ss;
      ss << "Q" << ++i;
      string ident = ss.str();
      graph.buildQueryDependencies(p, ident);
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

  string evaluateRulesStart(vector<Rule>& rules, set<string>& query_history, bool& keep_going){
    stringstream ss;
    // int num_of_passes = 0;
    int previous_count = db.getCount();

    while (true){
      previous_count = db.getCount();
      ss << evaluateRulesRec(rules, query_history, keep_going);
      if (previous_count == db.getCount()){
        break;
      }
    }
    // cout << "Converged after " << num_of_passes << " passes through the Rules." << endl;
    return ss.str();
  }

  string evaluateRulesRec(vector<Rule>& rules, set<string>& query_history, bool& keep_going){
    // num_of_passes++;
    stringstream ss;
    for (Rule r : rules){
      if (r.getPredicate().getIdent() != ""){
        ss << "    " << r.toString() << endl;
      }
      if (keep_going){
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
    return ss.str();
  }


  void dfs(Node& node, int& number, vector<string>& order, set<Node>& nodes){
    if (!node.getMark()){
      node.setMark(true);
      for (string s : node.getDependencies()){
        dfs(graph.get(s), number, order, nodes);
      }
      node.setPost(number++);
      if (node.getIdent().substr(0,1) != "Q")
        order.push_back(node.getIdent());
      nodes.insert(node);
    }
  }

  string evaluateQueries(){
    stringstream ss;
    set<string> query_history;
    int i = 0;
    for (Predicate p : dp.getQueries()){
      ss << p.toString() << "?\n" << endl;

      stringstream ss2;
      ss2 << "Q" << ++i;
      string ident = ss2.str();

      vector<string> order;
      set<Node> nodes;
      map<string, set<string>> edges;
      int post = 1;

      graph.resetMarks();
      dfs(graph.get(ident), post, order, nodes);


      ss << "  Postorder Numbers" << endl; 
      ss << graph.postOrder(nodes) << endl;

      ss << graph.ruleOrder(order) << endl;


      ss << graph.backwardEdges(edges) << endl;

      ss << "  Rule Evaluation" << endl;
      vector<Rule> new_rules;
      for (string s : order){
        new_rules.push_back(graph.get(s).getRule());
      }

      bool keep_going = true;
      if (query_history.count(p.getIdent())){

        keep_going = false;
      }
      if (edges.size() > 0){
        ss << evaluateRulesStart(new_rules, query_history, keep_going) << endl;
      }
      else{
        ss << evaluateRulesRec(new_rules, query_history, keep_going) << endl;
      }
      query_history.insert(p.getIdent());
      ss << db.selects(p) << endl;
    }
    return ss.str();
  }

  string toStringDb(){
    return db.toString();
  }
  string toStringGraph(){
    return graph.toString();
  }
};