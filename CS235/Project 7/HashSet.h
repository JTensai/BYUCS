#include <iostream>
#include <string>
#include <sstream>
#include "LinkedList.h"

// using namespace std;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;

template <typename ItemType>

class HashSet {

  private:
    LinkedList<ItemType>* table;
    int tableSize;
    int size;

  public:
    HashSet() :
      tableSize(0),
      size(0),
      table(NULL)
    {}


  void add(const ItemType& item) {
    if (find(item))
      return;
    if (size == tableSize)
      grow();

    unsigned index = hashCode(item);
    table[index].append(item);
    size++;
    // cout << "Added: " << table[index].get_first_item() << endl;
  }

  void remove(const ItemType& item) {
    if (!find(item))
      return;

    unsigned index = hashCode(item);
    table[index].remove_item(item);
    size--;

    if (size == tableSize/2)
      shrink();
  }

  bool find(const ItemType& item) {
    // cout << "Checking if " << item << " exists" << endl;
    if (tableSize == 0)
      return false;
    unsigned index = hashCode(item);
    return table[index].find(item);
  }

  unsigned hashCode( const string& s ) {
    unsigned hashIndex = 0;
    string str = s;
    for(char& c : str){
      hashIndex = (hashIndex * 31) + c;
    }
    // cout << s << ": number: " << hashIndex << " % " << tableSize << " = index: " << hashIndex % tableSize << endl;
    return hashIndex % tableSize;
  }

  void grow(){
    int newTableSize = (tableSize*2)+1;
    // cout << "Table grown from " << tableSize << " to " << newTableSize << endl;
    rehash(newTableSize);
  }

  void shrink(){
    int newTableSize = tableSize/2;
    // cout << "Table shrunk from " << tableSize << " to " << newTableSize << endl;
    rehash(newTableSize);
  }

  void rehash(int newTableSize){
    int oldTableSize = tableSize;
    LinkedList<ItemType>* newTable = new LinkedList<ItemType>[newTableSize];
    tableSize = newTableSize;
    // Loops through each LinkedList in the table
    for (int i = 0; i < oldTableSize; i++){
      while(table[i].get_first()){
        ItemType item = table[i].get_first_item();
        unsigned index = hashCode(item);
        newTable[index].append(item);
        table[i].remove(0);
      }
    } 
    for (int i = 0; i < oldTableSize; i++){
      table[i].clear_all();
    }
    delete[] table;
    table = newTable;
  }

  string print(){
    stringstream ss;
    for (int i = 0; i < tableSize; i++){
      if (table[i].size() == 0)
        ss << "\nhash " << i << ":";

      int bucket_count = 0;
      for (int j = 0; j < table[i].size(); j++){
        if (bucket_count == 0){
          ss << "\nhash " << i << ":";
        }
        ss << " " << table[i].get_item(j);
        bucket_count++;
        if (bucket_count == 8){
          bucket_count = 0;
        }
      }
    } 
    ss << endl;
    return ss.str();  
  }

  void clear(){  
    for (int i = 0; i < tableSize; i++){
      table[i].clear_all();
    }
    tableSize = 0;
    size = 0;
  }


  ~HashSet() {  
    clear();
    delete[] table;
  }
};