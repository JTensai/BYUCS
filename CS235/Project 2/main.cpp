#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <cctype>
#include <algorithm>

using namespace std;

string parse(string w){
  int i = 0;
  for(char& l : w) {
    if (isalpha(l)){
      i++;
    }
    else {
      l = ' ';
    }
  }
  return w;
}

string toLower(string w){
  for(char& l : w){
    l = tolower(l);
  }
  return w;
}

set<string> create_dictionary(char* dict_file){
  set<string> words;
  string w;
  ifstream in;

  in.open(dict_file);

  while (in >> w){
    string parsed_string = toLower(parse(w));
    stringstream ss(parsed_string);
    string word;
    while (ss >> word){
      if (!word.empty())
        words.insert(word);
    }
  }
  in.close();
  return words;
}

map<string,list<int>> spell_check(set<string> dict, char* file){
  map<string,list<int>> mispelled;
  list<int> lines;
  string line_string;
  ifstream in;
  int line_num = 0;

  in.open(file);

  // loop through each line of the document
  while (getline(in, line_string)){
    line_num++;
    stringstream ss(line_string);
    // loop through each word of a line
    string stream_word;
    while (ss >> stream_word){
      // loop through a word and parse it
      string parsed_string = toLower(parse(stream_word));

      stringstream ws(parsed_string);
      string word;
      while (ws >> word){
        if (!word.empty()){
          // compare word here to dictionary
          if (dict.count(word)){
            //word exists in dictionary
          }
          else{
            //what happens when mispelled word is found
            list<int>lines = mispelled[word];
            lines.push_back(line_num);
            mispelled[word] = lines;
          }
        }
      }
    }
  }

  in.close();

  return mispelled;
}

void print_mispelled(map<string, list<int>> mispelled, char* file_name){

  ofstream out;

  out.open(file_name);

  map<string, list<int>>::iterator i = mispelled.begin();
  while (i != mispelled.end()){
    out << i->first << ":";
    list<int>::iterator s = i->second.begin();
    while (s != i->second.end()){
      out << " " << *s;
      s++;
    }
    out << endl;
    i++;
  }

  out.close();  
}

int main(int argc, char* argv[]){
  set<string> dictionary = create_dictionary(argv[1]);
  map<string,list<int>> mispelled = spell_check(dictionary, argv[2]);

  print_mispelled(mispelled, argv[3]);

  return 0;
}