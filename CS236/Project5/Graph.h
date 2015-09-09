#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "Node.h"

using std::map;
using std::string;
using std::stringstream;

class Graph : public map<string, Node>{
  private:
  map<string, Node> values;

  public:
  Graph(){}

  Graph(map<string, Node> values){
    setValues(values);
  }

  void setValues(map<string, Node> values){
    this->values = values;
  }

  map<string, Node> getValues() const{
    return values;
  }

  Node& get(string s){
    return values[s];
  }

  void addNode(Predicate p, string ident){
    // use rule passed in to make a node of dependencies
    Node n = Node(p, ident);
    values[ident] = n;

  }

  void addNode(Rule r, string ident){
    // use rule passed in to make a node of dependencies
    Node n = Node(r, ident);
    values[ident] = n;

  }

  void buildRuleDependencies(Rule r, string ident){
    for (Predicate p : r.getPredList()){
      for (auto& entry : getValues()){
        if (entry.second.getRule().getPredicate().getIdent() == p.getIdent()){
          values[ident].addDependency(entry.second.getIdent());
        }
      }
    }
  }

  void buildQueryDependencies(Predicate p, string ident){
    for (auto& entry : getValues()){
      // cout << entry.second.getRule().getPredicate().getIdent() << " =? " << p.getIdent() << endl;
      if (entry.second.getRule().getPredicate().getIdent() == p.getIdent()){
        values[ident].addDependency(entry.second.getIdent());
      }
    }
  }

  void resetMarks(){
    for (auto& entry : values){
      entry.second.setMark(false);
    }
  }

  string postOrder(set<Node>& nodes){
    stringstream ss;
    for(Node n : nodes){
      ss <<  "    " << n.getIdent() << ": " << n.getPost() << endl;
    }
    return ss.str();
  }
  
  string ruleOrder(vector<string> order){
    stringstream ss;
    ss << "  Rule Evaluation Order" << endl;

    for (string s : order){
      if (s.substr(0,1) != "Q")
        ss << "    " << s << endl;
    }
    return ss.str();
  }
  
  string backwardEdges(map<string, set<string>>& edges){
    stringstream ss;
    ss << "  Backward Edges" << endl;

    for (auto& entry : values){
      Node n1 = entry.second;
      set<string> nodes;
      if (n1.getMark()){
        for (string s : n1.getDependencies()){
          Node n2 = get(s);
          if (n1.getPost() <= n2.getPost()){
            nodes.insert(s);
          }
        }
        if (nodes.size() > 0)
          edges[n1.getIdent()] = nodes;
      }
    }

    for (auto& entry : edges){
      ss << "    " << entry.first << ": ";
      for (string s : entry.second){
        ss << s << " ";
      }
      ss << endl;
    }

    return ss.str();
  }

  string printValues(){
    for (auto& entry : values){
      cout << "-----------------------------------------------"<< endl;
      cout << entry.first << endl;
      for (string s : entry.second.getDependencies()){
        cout << s << "\t";
      }
      cout << "\n\n";
    }
  }

  string toString(){
    stringstream ss;
    ss << "Dependency Graph" << endl;
    for (auto& s : values){
      ss << "  " << s.second.toString() << endl;;
    }
    return ss.str();
  }
};