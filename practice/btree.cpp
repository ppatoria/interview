#include <iostream>

struct node{
  node* left;
  node* right;
  int data;
};

class btree{
 public:
  btree() : head(nullptr){}
  node* add(int x){
    return add(x, head);
  }

  node* remove(int x) {
    return remove(x, head);
  }

  node* search(int x){
    return search(x, head);
  }

  void preorder(){
    preorder(head);
  }

  void inorder(){
    inorder(head);
  }

  void postorder(){
    postorder(head);
  }
  
  
 private:
  node* head;
  node* add(int x, node*& current);
  node *remove(int x, node*& current);
  node *search(int x, node* current);
  void preorder(node* current);
  void inorder(node* current);
  void postorder(node* current);
  
};

node* btree::add(int x, node *&current) {
  if(current == nullptr){
    current = new node
              {
                nullptr, nullptr, x
              };
    return current;
  }
  if(x < current->data ){
    return add(x, current->left);
  } else if(x > current->data){
    return add(x, current->right);
  }else{
    return current;
  }
}

void btree::preorder(node* current){
  if(current == nullptr) {
    return;
  }
  
  std::cout << current->data << std::endl;
  preorder(current->left);
  preorder(current->right);
}

void btree::inorder(node* current){
  if(current == nullptr){
    return;
  }

  inorder(current->left);
  std::cout << current->data << std::endl;
  inorder(current->right);
};

void btree::postorder(node *current) {
  if(current == nullptr){
    return;
  }

  postorder(current->left);
  postorder(current->right);
  std::cout << current->data << std::endl;
};

node* btree::search(int x, node* current){
  if(current == nullptr){
    return current;
  }

  if(x == current->data){
    return current;
  } else if (x < current->data) {
    return search(x, current->left);
  } else {
    return search(x, current->right);
  }
}



  
  


  int main() {
    btree bt;
    bt.add(31);
    bt.add(11);
    bt.add(21);
    bt.add(41);

    bt.preorder();
    bt.inorder();
    bt.postorder();

    node* searchResult = bt.search(21);
    if(searchResult != nullptr){
      std::cout << "search result: " << searchResult->data << std::endl;
    } else {
      std::cout << "not found" << std::endl;
    }
    
    
    
    

    return 0;
}
