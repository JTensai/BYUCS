#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "Interpreter.h"

using namespace std;

int main(int argc, char* argv[]){
  ofstream out;
  out.open(argv[2]);
  // stringstream ss;
  try{
    Interpreter interpreter = Interpreter(argv[1]);

    interpreter.buildGraph();

    string text;
    text = interpreter.toStringGraph();
    out << text << endl;
    // ss << text << endl;

    interpreter.createSchemes();
    interpreter.createTuples();

    text = interpreter.evaluateQueries();
    out << text;
    // ss << text;

    // cout << ss.str();
  }
  catch (const std::invalid_argument& e){
    out << "Failure!\n  " << e.what() << endl;
  }
  out.close();

  return 0;
}