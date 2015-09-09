#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>
#include "Token.h"

using namespace std;

class Scanner{
  private:
  vector<Token> tokens;

  public:
  Scanner(char* source){
    tokens = createTokens(source);
  }

  vector<Token> getTokens(){
    return tokens;
  }

  map<string, string> createTypes(){
    map<string, string> types;
    types[","] = "COMMA";
    types["."] = "PERIOD";
    types["?"] = "Q_MARK";
    types["("] = "LEFT_PAREN";
    types[")"] = "RIGHT_PAREN";
    types[":"] = "COLON";
    types[":-"] = "COLON_DASH";
    types["Schemes"] = "SCHEMES";
    types["Facts"] = "FACTS";
    types["Rules"] = "RULES";
    types["Queries"] = "QUERIES";

    return types;
  }

  void processWord(string word, int line, vector<Token>* tokens, map<string, string>& types){
    Token word_token ("","",-1);
    if (word[0] == '\''){
      word_token.setType("STRING");
      word.erase(0,1);
    }
    else if (types.count(word)){
      word_token.setType(types[word]);
    }
    else{
      if (!isalpha(word[0])){
        stringstream line_num;
        line_num << line;
        throw std::invalid_argument( line_num.str() );
      }
      else
        word_token.setType("ID");
    }
    word_token.setValue(word);
    word_token.setLine(line);
    tokens->push_back(word_token);
  }

  void handleString(ifstream& in, stringstream& word, char& ch, int& line){
    // in.get();
    bool cont = true;
    while (cont){
      char next = char(in.get());
      if (next == '\''){
        cont = false;
        break;
      }
      word << next;  
      if (ch == '\n' || in.eof()){
        stringstream line_num;
        line_num << line;
        throw std::invalid_argument( line_num.str() );
      }
    } 
  }

  void handleNonAlphaNum(ifstream& in, stringstream& word, char& ch, int& line, vector<Token>& tokens, map<string, string>& types){
    if (word.str() != ""){
      processWord(word.str(), line, &tokens, types);
    }
    word.str("");
    word << ch;
    // Handles the COLON_DASH case
    if (ch == ':'){
      char next = char(in.peek());
      if (next == '-'){
        word << char(in.get());
      }
    }

    // Handles the STRING case
    if (ch == '\''){
      handleString(in, word, ch, line);
    }

    processWord(word.str(), line, &tokens, types);
    word.str("");
    word.clear();
  }

  void handleValidInput(ifstream& in, stringstream& word, char& ch, int& line, vector<Token>& tokens, map<string, string>& types){
    if (isalpha(ch) || isdigit(ch)){
      word << ch;
      if (!isalpha(char(in.peek())) && !isdigit(char(in.peek()))){
        processWord(word.str(), line, &tokens, types);
        word.str("");
      }     
    }
    // non alpha found
    else{
      handleNonAlphaNum(in, word, ch, line, tokens, types);
    }
  }

  vector<Token> createTokens(char* query_file){
    map<string, string> types = createTypes();
    vector<Token> tokens;

    ifstream in;
    in.open(query_file);

    int line = 1;
    char ch;
    stringstream word;

    try{
      while(in.get(ch)){
        // Comment found, skip line
        if (ch != '#'){
          // Skips whitespace
          if (!isspace(ch)){
            // Alpha found
            handleValidInput(in, word, ch, line, tokens, types);
          }
          if (ch == '\n') 
            line++;
        }
        else{
          string comment;
          getline(in, comment);
          line++;
        }
      }
    }
    catch (const std::invalid_argument& e){
    }

    in.close();
    return tokens;
  }

  string toString(){
    stringstream out;
    for (Token token : tokens){
      out << token.toString() << endl;
    }
    return out.str();
  }
};