#include <iostream>

struct node{
  node* next;
  int data;
};


class list {
 public:
  list() : head(nullptr){}
  void insert(int x);
  bool remove(int x);
  node* search(int x);
  void print();
  ~list(){
    while(head != nullptr){
      node* temp = head;
      head = head->next;
      delete temp;
    }
  }

 private:
  node* head;
  void insert(int x, node*& current);
  bool remove(int x, node*& current);
  node* search(int x, node* current);
  void print(node* current);
};

void list::insert(int x){
  insert(x, head);
};


void list::insert(int x, node*& current) {
  if(current == nullptr){
    auto new_node = new node
           {
             nullptr, x
           };
    current = new_node;
    return;
  } else {
    return insert(x, current->next);
  }
};


node* list::search(int x){
  return search(x, head);
};


node* list::search(int x, node* current){
  if(current == nullptr){
    return nullptr;
  }
  if(current->data == x){
    return current;
  } else {
    return search(x,current->next);
  }
};




void list::print(){
  print(head);
}

void list::print(node* current){
  if(current != nullptr){
    std::cout << current->data << std::endl;
    return print(current->next);
  }
  return;
}

bool list::remove(int x){
  return remove(x, head);
}


bool list::remove(int x, node*& current) {
  if(current == nullptr){
    return false;
  }

  if(current->data == x){
    node* temp = current;
    current = current->next;
    delete temp;
    return true;
  }

  return remove(x, current->next);

}


int main(){
  list l;
  l.insert(11);
  l.insert(21);
  l.insert(31);
  l.print();
  node* result = l.search(11);
  if(result != nullptr){
    std::cout << "search result: " << result->data << std::endl;
  } else {
    std::cout << "search result: nullpt" << std::endl;
  }

  if(l.remove(21)){
    std::cout << "removed node with 21" << std::endl;
  } else {
    std::cout << "not able to find the node" << std::endl;
  }

  l.print();
  
  return 0;
}
