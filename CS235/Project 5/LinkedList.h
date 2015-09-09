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

    void insert(int position, const ItemType& item){
      Node* n = new Node();
      n->item = item;
      // First node
      if (count == 0){
        first = n;
        last = n;
      }
      // Add to the front of the list
      else if (position <= 0){
        n->next = first;
        first->prev = n;
        first = n;
      }
      // needs to be added to end of list (position >= count)
      else if (position > count-1){
        n->prev = last;
        last->next = n;
        last = n;
      }
      // Inserting somewhere in the middle of the list
      else{
        Node* move_me = get_node(position);
        Node* previous = move_me->prev;
        n->prev = previous;
        n->next = move_me;
        previous->next = n;
        move_me->prev = n; 
      }
      count++;
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
/*
    ItemType get(int index) const{
      LinkedList<ItemType>::Iterator i = begin();
      int count = 0;
      while(!i.end()){
        if (count == index)
          return *i;
        count++;
      }
      return -1;  
    }*/
    
    int find(const ItemType& item) const {
      LinkedList<ItemType>::Iterator i = begin(); 
      int index = 0;
      for(i; i != end(); ++i){
        if (item == *i)
          return index;
        else
          index++;
      }
      return -1;
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

    ItemType remove(int position) {
      Node* remove_me = get_node(position);
      if (remove_me == NULL){
        cout << "Cannot delete null node" << endl;
      }
      ItemType item;
      // Does nothing if an invalid position is given
      if (remove_me != NULL){
        item = remove_me->item;
        // Clears the list when there is only 1 item
        if (count == 1){
          Node* temp = first;
          first = last = NULL;
          delete temp;
        }
        // More than one item in the list
        else{
          // Remove from the front of the list
          if (position == 0){
            first = remove_me->next;
            first->prev = NULL;
            delete remove_me;
          }
          // needs to be removed from the end of the list (position >= count)
          else if (position == count-1){
            last = remove_me->prev;
            last->next = NULL;
            delete remove_me;
          }
          // Removing somewhere in the middle of the list
          else{
            remove_me->next->prev = remove_me->prev;
            remove_me->prev->next = remove_me->next;
            delete remove_me;
          }
        }
        count--; 
      }      

      return item;
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

    void swap(LinkedList& list) {
      std::swap(count, list.count);
      std::swap(first, list.first);
      std::swap(last, list.last);
    }


};
