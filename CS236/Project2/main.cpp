#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>
#include "Scanner.h"
#include "DatalogProgram.h"

using namespace std;

int main(int argc, char* argv[]){
  

  ofstream out;
  out.open(argv[2]);
  try{
    Scanner scanner = Scanner(argv[1], argv[2]);

    // cout << scanner.toString() << endl;
    vector<Token> tokens = scanner.getTokens();

    DatalogProgram dp = DatalogProgram(tokens);
    out << dp.toString();
  }
  catch (const std::invalid_argument& e){
    out << "Failure!\n  " << e.what() << endl;
  }
  out.close();
  return 0;
}