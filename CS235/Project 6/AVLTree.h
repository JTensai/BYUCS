#include <iostream>
#include <string>
#include <sstream>

// using namespace std;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;


template <typename ItemType>

class AVLTree {

  private:
    struct Node{
      ItemType item;
      Node* left;
      Node* right;
      int height;
      Node(const ItemType& item) :
        item(item),
        left(NULL),
        right(NULL),
        height(1)
      {}
    };

    int size;
    Node* root;

  public:
    AVLTree() :
      size(0),
      root(NULL)
    {}

    void add(ItemType& item){
      if ( find(item) ){
        // item already exists in tree
        return;
      }
      add(item, root);
    }

    Node* add(ItemType& item, Node* node){
      // Root Node
      if (node == NULL){
        Node* n = new Node(item);
        if (size == 0)
          root = n;
        size++;
        return n;
      }  
      // Add to left child
      if ( item < node->item ){
        node->left = add(item, node->left);
      }
      // Add to right child
      else if ( item > node->item){
        node->right = add(item, node->right);
      }
      balance(node);
      node->height = get_height(node);
      return node;
    }

    int get_height(Node* node){
      if (node == NULL)
        return 0;
      else{
        int left_height = get_height(node->left);
        int right_height = get_height(node->right);

        if (left_height > right_height)
          return 1 + left_height;
        else
          return 1 + right_height;
      }
    }
    
    bool find(const ItemType& item) const {
      return find(item, root);
    }
    
    bool find(const ItemType& item, Node* node) const {
      if (node == NULL) {
        return false;
      }
      else if (item < node->item) {
        find(item, node->left);
      }
      else if (item > node->item) {
        find(item, node->right);
      }
      else{
        return true;
      }
    }

    void remove(const ItemType& item){
      if ( !find(item) ){
        return;
      }
      root = remove(item, root);
    }

    Node* remove(const ItemType& item, Node* node) {
      if (item < node->item) {
        node->left = remove(item, node->left);
        balance(node);
        node->height = get_height(node);
      }
      else if (item > node->item) {
        node->right = remove(item, node->right);
        balance(node);
        node->height = get_height(node);
      }
      else if (item == node->item) {
        node = remove_found_node(node);
      }
      // balance(node);
      return node;     
    }

    Node* remove_found_node(Node* node){
      // No children, simply delete node
      if (node->left == NULL && node->right == NULL){
        delete node;
        size--;
        return NULL;
      }
      // Left child only
      else if (node->right == NULL){
        Node* temp = node->left;
        delete node;
        size--;
        return temp;
      }
      // Right child only
      else if (node->left == NULL){
        Node* temp = node->right;
        delete node;
        size--;
        return temp;
      }
      // Two children
      else {
        Node* min_node = find_min(node->right);
        node->item = min_node->item;
        node->right = remove(node->item, node->right);
        balance(node);   
        node->height = get_height(node);
        return node;
      }
    }

    Node* find_min(Node* node){
      while (node->left){
        node = node->left;        
      }
      return node;
    }

    int balanceCheck(Node* node){
      int height_left, height_right;
      node->left ? height_left = node->left->height : height_left = 0;
      node->right ? height_right = node->right->height : height_right = 0;
      return height_left - height_right;
    }

    void balance(Node*& node){
      int node_balance = balanceCheck(node);
      if (node_balance == 2) {
        if (balanceCheck(node->left) == -1) {
          node->left = rotate_to_left(node->left);
        }
        node = rotate_to_right(node);
      } 
      else if (node_balance == -2){
        if (balanceCheck(node->right) == 1) {
          node->right = rotate_to_right(node->right);
        }
        node = rotate_to_left(node);
      }
      else {
      }
    }

    Node* rotate_to_right(Node* node){
      Node* newParent;
      newParent = node->left;
      node->left = newParent->right;
      newParent->right = node;
      if (node == root)
        root = newParent;
      node = newParent;

      if (node->left != NULL)
        node->left->height = get_height(node->left);
      if (node->right != NULL)
        node->right->height = get_height(node->right);

      return node;
    }

    Node* rotate_to_left(Node* node){
      Node* newParent;
      newParent = node->right;
      node->right = newParent->left;
      newParent->left = node;
      if (node == root)
        root = newParent;
      node = newParent;

      if (node->left != NULL)
        node->left->height = get_height(node->left);
      if (node->right != NULL)
        node->right->height = get_height(node->right);

      return node;
    }

    int maxHeight(Node* node_left, Node* node_right){
      int height_left, height_right;
      node_left ? height_left = node_left->height : height_left = 0;
      node_right ? height_right = node_right->height : height_right = 0;

      int height;
      height_left >= height_right ? height = height_left + 1 : height = height_right + 1;

      return height;
    }

    string print_all(){
      stringstream ss;
      ss << LevelOrder(root);
      return ss.str();
    }

    string LevelOrder(Node* root_node){
      stringstream ss;
      if (root == NULL){
        // cout <<"Tree is empty" << endl;
      }
      else{          
        int h = root_node->height;
        for(int i = 0; i <= h-1; i++){
          int node_count = 0;
          ss << "level " << i << ":";
          ss << printLevel(root_node, i, node_count, i);
          ss << endl;
        }
      }
      return ss.str();
    }

    string printLevel(Node* node, int level, int &node_count, int passed_level){
      stringstream ss;
      if(node == NULL)
        return "";
      if(level == 0){
        node_count++;
        if (node_count >= 9){
          node_count = 1;
          ss << "\nlevel " << passed_level << ":";
        }
        ss << " " << node->item << "(" << node->height << ")";// << "[" << node_count << "]";
      }
      else if (level > 0)
      {
        ss << printLevel(node->left, level-1, node_count, passed_level);
        ss << printLevel(node->right, level-1, node_count, passed_level);
      }
      return ss.str();
    }

    void clear_all(){
      while(root){
        // print_all();
        remove(root->item);
      }
    }    

    ~AVLTree() {
      clear_all();
    }
};
