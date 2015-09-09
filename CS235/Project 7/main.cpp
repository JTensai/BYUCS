#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "HashSet.h"

using namespace std;

string clear_print(HashSet<string> &table, string command){
  stringstream ss;
  if (command == "clear"){
    ss << "clear" << endl;
    table.clear();
  }
  if (command == "print"){
    ss << "print";
    string results;
    results = table.print();
    ss << results;
  }
  return ss.str();
}

int main(int argc, char* argv[]){

  ifstream in;
  ofstream out;
  in.open(argv[1]);
  out.open(argv[2]);  

  string line_string;
  HashSet<string> table;

  while (getline(in, line_string)){
    stringstream ss(line_string);
    string command, item;
    while (ss){
      ss >> command;
      
      if (command == "add"){
        ss >> item;
        out << "add " << item << endl;
        table.add(item);
        // cout << "\n\n\n";
        break;
      }
      if (command == "remove"){
        ss >> item;
        out << "remove " << item << endl;
        table.remove(item);
        break;
      }
      if (command == "find"){
        ss >> item;
        string result = "false";
        if (table.find(item))
          result = "true";
        out << "find " << item << " " << result << endl;
        break;
      }
      else{
        out << clear_print(table, command);
        break;
      }
    }
  }
  in.close();

  return 0;
}

