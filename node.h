#ifndef NODE_H
#define NODE_H
#include <iostream>

class Node
{
 public:
  // constructors and destructors
  Node();
  Node(char);
  ~Node();

  // functions (getters)
  Node* getNext(); // returns next node pointer
  Node* getLeft(); // returns left child
  Node* getRight(); // returns right child
  char getValue(); // returns data value

  // functions (setters)
  void setNext(Node*); // establish next node pointer
  void setLeft(Node*); // establish left child
  void setRight(Node*); // establish right child
  void setValue(char); // establish data value
  
 private:
  // variables
  char data;
  Node* next;
  Node* left;
  Node* right;
  
};
#endif
