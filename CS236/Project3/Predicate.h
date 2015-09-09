#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Parameter.h"

using std::string;
using std::stringstream;

class Predicate {
  private:
  string ident;
  vector<Parameter> params;

  public:
  Predicate(){}

  string getIdent(){
    return ident;
  }

  vector<Parameter> getParams(){
    return params;
  }

  vector<string> getParamValues(){
    vector<string> values;
    for (Parameter p : params){
      values.push_back(p.getValue());
    }
    return values;
  }

  void create(vector<Token>& tokens){
    if (tokens.front().getType() == "ID"){
      ident = tokens.front().getValue();
      tokens.erase(tokens.begin());
      if (tokens.front().getType() == "LEFT_PAREN"){
        tokens.erase(tokens.begin());
        bool cont = true;
        while (tokens.size() > 0){
          if (tokens.front().getType() == "RIGHT_PAREN"){
            break;
          }
          if (cont == false && tokens.front().getType() != "RIGHT_PAREN"){
            throw std::invalid_argument( tokens.front().toString() );
          }
          handleParams(tokens, cont);
        }
        if (params.size() == 0){
          throw std::invalid_argument( tokens.front().toString() );
        }
        tokens.erase(tokens.begin());
        return;
      }
      else{
        throw std::invalid_argument( tokens.front().toString() );
      }
    }
    else{
      throw std::invalid_argument( tokens.front().toString() );
    }
  }

  void handleParams(vector<Token>& tokens, bool& cont){
    if (tokens.front().getType() == "STRING"){
      params.push_back(Parameter("STRING", "'"+tokens.front().getValue()+"'"));
      tokens.erase(tokens.begin());
      cont = false;
    }
    else if (tokens.front().getType() == "ID"){
      params.push_back(Parameter("", tokens.front().getValue()));
      tokens.erase(tokens.begin());
      cont = false;
    }
    else{
      throw std::invalid_argument( tokens.front().toString() );
    }

    if (tokens.front().getType() == "COMMA"){
      tokens.erase(tokens.begin());
      cont = true;
      if (tokens.front().getType() != "STRING" && tokens.front().getType() != "ID"){
        throw std::invalid_argument( tokens.front().toString() );
      }
    }
  }

  // void create

  string toString(){
    stringstream out;
    out << ident << "(";
    int i = 0;
    for (Parameter p : params){
      if (i++ == 0){
        out << p.getValue();
      }
      else{
        out << "," << p.getValue();
      }
    }
    out << ")";
    return out.str();
  }
};