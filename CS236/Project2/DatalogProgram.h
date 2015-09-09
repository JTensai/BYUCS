#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "Predicate.h"
#include "Rule.h"
#include "Parameter.h"

using std::string;
using std::stringstream;

class DatalogProgram {
  private:
    vector<Token> tokens;
    vector<Predicate> schemes;
    vector<Predicate> facts;
    vector<Rule> rules;
    vector<Predicate> queries;
    set<string> params;

  public:
  DatalogProgram(vector<Token> tokens){
    setTokens(tokens);
    parse(tokens);
  }

  vector<Token> getTokens() const { return tokens; }

  void setTokens(vector<Token> tokens){
    this->tokens = tokens;
  }
  void setSchemes(vector<Predicate> schemes){
    this->schemes = schemes;    
  }
  void setFacts(vector<Predicate> facts){
    this->facts = facts;    
  }
  void setRules(vector<Rule> rules){
    this->rules = rules;    
  }
  void setQueries(vector<Predicate> queries){
    this->queries = queries;    
  }

  void parse(vector<Token> tokens){
    int i = 0;
    if (tokens.front().getType() == "SCHEMES" && i == 0){
      tokens.erase(tokens.begin());
      setSchemes(parseSchemes(tokens));
      i++;
      if (schemes.size() == 0){
        throw std::invalid_argument( tokens.front().toString() );
      }
    }
    else 
      throw std::invalid_argument( tokens.front().toString() );

    if (tokens.front().getType() == "FACTS" && i == 1){
      tokens.erase(tokens.begin());
      setFacts(parseFacts(tokens));
      i++;
    }
    else 
      throw std::invalid_argument( tokens.front().toString() );

    parse2(tokens, i);
  }

  void parse2(vector<Token>& tokens, int& i){
    if (tokens.front().getType() == "RULES" && i == 2){
      tokens.erase(tokens.begin());
      setRules(parseRules(tokens));
      i++;
    }
    else 
      throw std::invalid_argument( tokens.front().toString() );

    if (tokens.front().getType() == "QUERIES" && i == 3){
      tokens.erase(tokens.begin());
      setQueries(parseQueries(tokens));
      i++;
      if (queries.size() == 0){
        throw std::invalid_argument( tokens.front().toString() );
      }
    }
    else 
      throw std::invalid_argument( tokens.front().toString() );

    if (tokens.size() > 0){
      throw std::invalid_argument( tokens.front().toString() );
    }
    createParamsList();
    createParamsList2();
  }

  vector<Predicate> parseSchemes(vector<Token> &tokens){
    vector<Predicate> predicates;
    if (tokens.front().getType() == "COLON"){
      tokens.erase(tokens.begin());
      while (tokens.size() > 0){
        if (tokens.front().getType() == "FACTS"){
          break;
        }
        predicates.push_back(createPredicate(tokens, "SCHEMES"));
      }
    }
    else{
      throw std::invalid_argument( tokens.front().toString() );
    }
    return predicates;
  }

  vector<Predicate> parseFacts(vector<Token> &tokens){
    vector<Predicate> predicates;
    if (tokens.front().getType() == "COLON"){
      tokens.erase(tokens.begin());
      while (tokens.size() > 0){
        if (tokens.front().getType() == "RULES"){
          break;
        }
        predicates.push_back(createPredicate(tokens, "FACTS"));
      }
    }
    else{
      throw std::invalid_argument( tokens.front().toString() );
    }
    return predicates;
  }

  vector<Rule> parseRules(vector<Token> &tokens){
    vector<Rule> rules;
    if (tokens.front().getType() == "COLON"){
      tokens.erase(tokens.begin());
      while (tokens.size() > 0){
        if (tokens.front().getType() == "QUERIES"){
          break;
        }
        rules.push_back(createRules(tokens));
      }
    }
    else{
      throw std::invalid_argument( tokens.front().toString() );
    }
    return rules;
  }

  vector<Predicate> parseQueries(vector<Token> &tokens){
    vector<Predicate> predicates;
    if (tokens.front().getType() == "COLON"){
      tokens.erase(tokens.begin());
      while (tokens.size() > 0){
        predicates.push_back(createPredicate(tokens, "QUERIES"));
      }
    }
    else{
      throw std::invalid_argument( tokens.front().toString() );
    }
    return predicates;
  }

  Predicate createPredicate(vector<Token> &tokens, string type){
    Predicate pred = Predicate();
    pred.create(tokens);
    if (type == "SCHEMES"){
      if (tokens.front().getType() == "FACTS" || tokens.front().getType() == "ID"){
        // tokens.erase(tokens.begin());
      }
      else{
        throw std::invalid_argument( tokens.front().toString() );
      }
    }
    else if (type == "FACTS"){
      if (tokens.front().getType() == "PERIOD"){
        tokens.erase(tokens.begin());
      }
      else {
        throw std::invalid_argument( tokens.front().toString() );
      }
    }
    else if (type == "QUERIES"){
      if (tokens.front().getType() == "Q_MARK"){
        tokens.erase(tokens.begin());
      }
      else{
        throw std::invalid_argument( tokens.front().toString() );
      }
    }
    return pred;
  }

  Rule createRules(vector<Token> &tokens){
    Rule rule = Rule();
    rule.create(tokens);

    if (tokens.front().getType() == "PERIOD"){
      tokens.erase(tokens.begin());
    }
    else{
      throw std::invalid_argument( tokens.front().toString() );
    }
    return rule;
  }

  void createParamsList(){
    for (Predicate pred : schemes){
      for (Parameter p : pred.getParams()){
        if (p.getType() == "STRING"){
          params.insert(p.getValue());
        }
      }
    }
    for (Predicate pred : facts){
      for (Parameter p : pred.getParams()){
        if (p.getType() == "STRING"){
          params.insert(p.getValue());
        }
      }
    }
  }

  void createParamsList2(){    
    for (Rule rule : rules){
      for (Parameter p : rule.getParams()){
        if (p.getType() == "STRING"){
          params.insert(p.getValue());
        }
      }
    }
    for (Predicate pred : queries){
      for (Parameter p : pred.getParams()){
        if (p.getType() == "STRING"){
          params.insert(p.getValue());
        }
      }
    }
  }

  string toString() const {
    stringstream out;
    out << "Success!" << endl;
    // if (schemes.size() > 0){
      out << "Schemes(" << schemes.size() << "):" << endl;
      for (Predicate pred : schemes){
        out << "  " << pred.toString() << endl;
      }
    // }
    // if (facts.size() > 0){
      out << "Facts(" << facts.size() << "):" << endl;
      for (Predicate pred : facts){
        out << "  " << pred.toString() << endl;
      }
    // }
    // if (rules.size() > 0){
      out << "Rules(" << rules.size() << "):" << endl;
      for (Rule rule : rules){
        out << "  " << rule.toString() << endl;
      }
    // }
    // if (queries.size() > 0){
      out << "Queries(" << queries.size() << "):" << endl;
      for (Predicate pred : queries){
        out << "  " << pred.toString() << endl;
      }
    // }
    // if (params.size() > 0){
      out << "Domain(" << params.size() << "):" << endl;
      for (string s : params){
        out << "  " << s << endl;
      }
    // }

    return out.str();
  }
};