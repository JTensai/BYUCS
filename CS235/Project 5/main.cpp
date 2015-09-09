#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "LinkedList.h"

using namespace std;

int main(int argc, char* argv[]){

  ifstream in;
  ofstream out;
  in.open(argv[1]);
  out.open(argv[2]);  

  string line_string;
  LinkedList<string> list;

  while (getline(in, line_string)){
    stringstream ss(line_string);
    string command, item;
    int position = -1;
    while (ss){
      ss >> command;
      if (command == "clear"){
        out << "clear" << endl;
        list.clear_all();
        break;
      }
      if (command == "insert"){
        ss >> position;
        ss >> item;
        list.insert(position, item);
        out << "insert " << position << " " << item << endl;
        break;
      }
      if (command == "remove"){
        ss >> position;
        out << "remove " << position << " " << list.remove(position) << endl;
        break;
      }
      if (command == "find"){
        ss >> item;
        out << "find " << item << " " << list.find(item) << endl;
        break;
      }
      if (command == "print"){
        out << "print" << endl;
        out << list.print_all();
        break;
      }
    }
  }
  in.close();

  return 0;
}

