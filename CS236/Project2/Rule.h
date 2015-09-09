#pragma once

#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;

class Rule {
  private:
    Predicate predicate;
    vector<Predicate> pred_list;
  public:
  Rule(){}

  Predicate getPredicate(){
    return predicate;
  }
  vector<Predicate> getPredList(){
    return pred_list;
  }

  void create(vector<Token>& tokens){
    Predicate pred = Predicate();
    pred.create(tokens);
    predicate = pred;
    if (tokens.front().getType() == "COLON_DASH"){
      tokens.erase(tokens.begin());
      while(tokens.size() > 0){
        if (tokens.front().getType() == "COMMA"){
          tokens.erase(tokens.begin());
        }
        else if (tokens.front().getType() == "PERIOD"){
          break;
        }
        Predicate pred = Predicate();
        pred.create(tokens);
        pred_list.push_back(pred);
      }
      if (pred_list.size() == 0){
        throw std::invalid_argument( tokens.front().toString() );
      }
    }
    else{
      throw std::invalid_argument( tokens.front().toString() );
    }
  }

  vector<Parameter> getParams(){
    vector<Parameter> params;
    for (Predicate pred : pred_list){
      for (Parameter p : pred.getParams()){
        params.push_back(p);
      }
    }
    return params;
  }

  string toString(){
    stringstream out;
    out << getPredicate().toString() << " :- ";
    int i = 0;
    for (Predicate pred : getPredList()){
      if (i++ == 0){
        out << pred.toString();
      }
      else{
        out << "," << pred.toString();
      }
    }
    return out.str();
  }
};