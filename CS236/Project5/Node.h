#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <set> 

using std::set;
using std::string;
using std::stringstream;

class Node : public set<string>{
  private:
  Rule rule;
  Predicate pred;
  string ident;
  set<string> dependencies;
  bool mark;
  int post;

  public:
  Node(){}

  Node(Predicate p, string s){
    setPost(0);
    setMark(false);
    setPredicate(p);
    setIdent(s);
    setRule(Rule());
  }

  Node(Rule r, string s){
    setPost(0);
    setMark(false);
    setRule(r);
    setIdent(s);
    setPredicate(Predicate());
  }

  Rule getRule(){
    return rule;
  }

  void setRule(Rule r){
    rule = r;
  }

  Predicate getPredicate(){
    return pred;
  }

  void setPredicate(Predicate p){
    pred = p;
  }

  string getIdent() const{
    return ident;
  }

  void setIdent(string s){
    ident = s;
  }

  void setMark(bool b){
    mark = b;
  }

  bool getMark(){
    return mark;
  }

  int getPost(){
    return post;
  }

  void setPost(int i){
    post = i;
  }

  set<string> getDependencies(){
    return dependencies;
  }

  void addDependency(string s){
    dependencies.insert(s);
  }

  void addDependency(set<string>& dep_set){
    for (string s : dep_set){
      dependencies.insert(s);
    }
  }


  string toString(){
    stringstream ss;
    ss << getIdent() << ": ";
    for (string s : dependencies){
      ss << s << " ";
    }
    return ss.str();
  }

  bool operator< (const Node n) const{
    return getIdent() < n.getIdent();
  }
};