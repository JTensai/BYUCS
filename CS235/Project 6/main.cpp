#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "AVLTree.h"

using namespace std;

string clear_print(AVLTree<string> &tree, string command){
  stringstream ss;
  if (command == "clear"){
    ss << "clear" << endl;
    tree.clear_all();
  }

  if (command == "print"){
    ss << "print" << endl;
    string results;
    results = tree.print_all();
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
  AVLTree<string> tree;

  while (getline(in, line_string)){
    stringstream ss(line_string);
    string command, item;
    while (ss){
      ss >> command;
      
      if (command == "add"){
        ss >> item;
        out << "add " << item << endl;
        tree.add(item);
        break;
      }
      if (command == "remove"){
        ss >> item;
        out << "remove " << item << endl;
        tree.remove(item);
        break;
      }
      if (command == "find"){
        ss >> item;
        string result = "false";
        if (tree.find(item)){
          result = "true";
        }
        out << "find " << item << " " << result << endl;
        break;
      }
      else{
        out << clear_print(tree, command);
        break;
      }
    }
  }
  in.close();

  return 0;
}

