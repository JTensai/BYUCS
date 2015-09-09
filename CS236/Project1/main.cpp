#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>
#include "Token.h"

using namespace std;

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

void processWord(string word, int line, vector<Token>* tokens, map<string, string>& types, ofstream& out){
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

  out << word_token.toString() << endl;
}

void handleString(ifstream& in, stringstream& word, char& ch, int& line){
  bool cont = true;
  while (cont){
    char next = char(in.get());
    if (next == '\n' || in.eof()){
      stringstream line_num;
      line_num << line;
      throw std::invalid_argument( line_num.str() );
    }
    if (next == '\''){
      cont = false;
      break;
    }
    word << next; 
  } 
}

void handleNonAlphaNum(ifstream& in, ofstream& out, stringstream& word, char& ch, int& line, vector<Token>& tokens, map<string, string>& types){
  if (word.str() != ""){
    processWord(word.str(), line, &tokens, types, out);
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

  processWord(word.str(), line, &tokens, types, out);
  word.str("");
  word.clear();
}

void handleValidInput(ifstream& in, ofstream& out, stringstream& word, char& ch, int& line, vector<Token>& tokens, map<string, string>& types){
  if (isalpha(ch) || isdigit(ch)){
    word << ch;
    if (!isalpha(char(in.peek())) && !isdigit(char(in.peek()))){
      processWord(word.str(), line, &tokens, types, out);
      word.str("");
    }     
  }
  // non alpha found
  else{
    handleNonAlphaNum(in, out, word, ch, line, tokens, types);
  }
}

vector<Token> createTokens(char* query_file, char* dest_file){
  map<string, string> types = createTypes();
  vector<Token> tokens;

  ofstream out;
  out.open(dest_file);
  ifstream in;
  in.open(query_file);

  int line = 1;
  char ch;
  stringstream word;

  try{
    while(in.get(ch)){
      if (ch != '#'){
        // Skips whitespace
        if (!isspace(ch)){
          // Alpha found
          handleValidInput(in, out, word, ch, line, tokens, types);
        }
        if (ch == '\n') 
          line++;
      }
      // Comment found, skip line
      else{
        string comment;
        getline(in, comment);
        line++;
      }
    }
    out << "Total Tokens = " << tokens.size() << endl;
  }
  catch (const std::invalid_argument& e){
    out << "Input Error on line " << e.what() << endl;
  }

  in.close();
  out.close();

  return tokens;
}


int main(int argc, char* argv[]){
  vector<Token> tokens = createTokens(argv[1], argv[2]);
  return 0;
}