#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <cctype>
#include <math.h>
// #include "../print.h"

using namespace std;

string toLower(string w){
  for(char& l : w){
    l = tolower(l);
  }
  return w;
}

void create_dictionary(char* dict_file, set<string>& dictionary){
  string w;
  ifstream in;

  in.open(dict_file);
  while (in >> w){
    if (w.size() > 3){
      dictionary.insert(toLower(w));
    }
  }
  in.close();
}

int find_board_size(char* board_file){
  ifstream in;
  string w;
  int count = 0;

  in.open(board_file);
  while (in >> w){
    count++;
  }
  in.close();

  int size = sqrt(count);

  return size;
}

void make_board(char* board_file, vector<vector<string>>& board){
  int board_size = find_board_size(board_file);

  string w;
  ifstream in;
  
  in.open(board_file);

  vector<string> row;
  int pos = 0;
  while(in >> w){
    pos++;
    row.push_back(toLower(w));
    if (pos % board_size == 0){
      board.push_back(row);
      row.clear();
    }
  }  

  in.close();
}

void print_results(vector<vector<string>> board, set<string> found_words, char* out_file){   
  ofstream out;
  out.open(out_file);
  for (int i = 0; i < board.size(); i++){
    for (int j = 0; j < board[i].size(); j++){
      out << board[i][j] << " ";
    }
    out << endl;
  }

  set<string>::iterator it = found_words.begin();
  out << endl;
  for (it; it != found_words.end(); it++){
    out << *it << endl;
  }
  out.close();
}

bool validate(int x, int y, string word_so_far, vector<vector<string>>& board, set<string>& dictionary, vector<vector<bool>>& exists, set<string>::iterator closest_word){

  // crops the closest word to be same length as word_so_far
  string sub_closest_word = *closest_word;
  sub_closest_word = sub_closest_word.substr(0, word_so_far.size());

  // if word_so_far has no matches in dictionary
  if (word_so_far != sub_closest_word){
    return false;
  }
  // if it is off the board, stop
  if (x < 0 || x > board.size()-1 || y < 0 || y > board.size()-1){
    return false;
  }
  // if it has been used, stop
  if (exists[x][y] == true){
    // cout << "letter already used" << endl;
    return false;
  }

  return true;
}

void find(int x, int y, string word_so_far, vector<vector<string>>& board, set<string>& dictionary, set<string>& found_words, vector<vector<bool>>& exists){
  set<string>::iterator closest_word = dictionary.lower_bound(word_so_far);
  set<string>::iterator end = dictionary.end();

  // checks if passes word is beyond the last word of the set
  if (closest_word == end){
    return;
  }

  // adds word to found_words set if a match in the dictionary is found
  if (word_so_far == *closest_word){
    found_words.insert(word_so_far);
  }

  if (!validate(x, y, word_so_far, board, dictionary, exists, closest_word)){
    return;
  }

  exists[x][y] = true;
  word_so_far += board[x][y];

  find(x-1, y-1, word_so_far, board, dictionary, found_words, exists);  // top left
  find(x, y-1, word_so_far, board, dictionary, found_words, exists);    // top middle 
  find(x+1, y-1, word_so_far, board, dictionary, found_words, exists);  // top right
  find(x-1, y, word_so_far, board, dictionary, found_words, exists);    // left middle
  find(x+1, y, word_so_far, board, dictionary, found_words, exists);    // right middle
  find(x-1, y+1, word_so_far, board, dictionary, found_words, exists);  // bottom left
  find(x, y+1, word_so_far, board, dictionary, found_words, exists);    // bottom middle
  find(x+1, y+1, word_so_far, board, dictionary, found_words, exists);  // bottom right

  exists[x][y] = false;

  return;
}

int main(int argc, char* argv[]){
  set<string> dictionary;
  vector<vector<string>> board;
  set<string> found_words;

  create_dictionary(argv[1], dictionary);
  make_board(argv[2], board);

  vector<vector<bool>> exists;
  for (int i = 0; i < board.size(); i++){
    vector<bool> row;
    for (int j = 0; j < board.size(); j++){
      row.push_back(false);
    }
    exists.push_back(row);
  }

  cout << endl;

  for (int col = 0; col < board.size(); col++){
    for (int row = 0; row < board.size(); row++){
      find(col, row, "", board, dictionary, found_words, exists);
    }
  }

  print_results(board, found_words, argv[3]);

  return 0;
}