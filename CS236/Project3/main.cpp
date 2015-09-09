#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "Interpreter.h"

using namespace std;

int main(int argc, char* argv[]){
  ofstream out;
  out.open(argv[2]);
  try{
    Interpreter interpreter = Interpreter(argv[1]);
    interpreter.createSchemes();
    interpreter.createTuples();
    out << interpreter.evaluateQueries();
    // interpreter.testProject();
    // out << interpreter.toString() << endl;
  }
  catch (const std::invalid_argument& e){
    out << "Failure!\n  " << e.what() << endl;
  }
  out.close();

  return 0;
}