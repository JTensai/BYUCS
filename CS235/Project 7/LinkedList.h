#include <iostream>
#include <string>
#include <sstream>

// using namespace std;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;


template <typename ItemType>

class LinkedList {

  private:
    struct Node{
      Node* prev;
      Node* next;
      ItemType item;
    };

    int count;
    Node* first;
    Node* last;

  public:
    LinkedList() :
      count(0),
      first(NULL),
      last(NULL)
    {}

    int size() const{ return count; }
    Node* get_first() const{ return first; }
    ItemType get_last_item() const{ return last->item; }
    ItemType get_first_item() const{ return first->item; }

    void append(const ItemType& item){
      Node* n = new Node();
      n->item = item;
      // First node
      if (count == 0){
        first = n;
        last = n;
      }
      // needs to be added to end of list (position >= count)
      else{
        n->prev = last;
        last->next = n;
        last = n;
      }
      count++;
    }

    ItemType get_item(int index) {
      return get_node(index)->item;
    }


    Node* get_node(int index) const{
      if (index < 0 || index >= count){
        cout << "Tried to get_node for invalid index" << endl;
        return NULL;
      }
      else if (index <= count/2){
        Node* temp = first;
        for(int i = 0; i <= index; i++){
          if (i > 0){
            temp = temp->next;            
          }
        }
        return temp;
      }
      else{
        Node* temp = last;
        for(int i = count-1; i >= index; i--){
          if (i < count-1){
            temp = temp->prev;
          }
        }
        return temp;
      }
    }
    
    bool find(const ItemType& item) const {
      Node* n = first;
      for (int i = 0; i < count; i++){
        if (n->item == item)
          return true;        
        n = n->next;
      }
      return false;
    }

    string print_all(){
      stringstream ss;
      LinkedList<ItemType>::Iterator i = begin(); 
      int index = 0;
      for(i; i != end(); ++i){
        ss << "node " << index << ": " << *i << endl; 
        index++;
      }
      return ss.str();
    }

    void remove_item(ItemType item){
      // cout << "deleting " << item << endl;
      Node* n = first;
      // Only one item in list      
      if (count == 1){
        first = last = NULL;
        delete n;
      }
      else{
        for (int i = 0; i < count; i++){
          // Item found, change pointers to remove it
          if (n->item == item){ 
            // First Item
            if (i == 0){
              first = n->next;
              first->prev = NULL;
              delete n;
            }
            // Last Item
            else if(i == count-1){
              last = n->prev;
              last->next = NULL;
              delete n;
            }
            // Middle Item
            else{
              n->next->prev = n->prev;
              n->prev->next = n->next;
              delete n;
            }
            break;
          }
          n = n->next;
        }
      }
      count--;
    }

    void remove(int position) {
      Node* n = first;
      // Only one item in list      
      if (count == 1){
        first = last = NULL;
        delete n;
      }
      else{
        first = n->next;
        first->prev = NULL;
        delete n;
      }
      count--;
    }

    void clear_all(){
      while (count > 0){
        remove(0);
      }      
    }

    class Iterator {

    private:

      const LinkedList& list;
      Node* node;

    public:

      Iterator(const LinkedList& l, Node* n) :
        list(l),
        node(n)
      {}


      ItemType operator * () const {
        return node->item;
      }

      void operator ++ () {
        node = node->next;
      }

      bool operator != (const Iterator& i) const {
        return node != i.node;
      }
    };

    Iterator begin() const {
      return Iterator(*this, first);
    }

    Iterator end() const {
      return Iterator(*this, NULL);
    }

    ~LinkedList() {
      while (count > 0){
        remove(0);
      }
    }

    LinkedList(const LinkedList& list) {
      count(list.count),
      first(list.first),
      last(list.last);
    }

    LinkedList& operator = (const LinkedList& list) {
      return *this;
    }
};
