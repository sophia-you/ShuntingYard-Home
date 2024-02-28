#include <iostream>
#include <cstring>
#include "node.h"

using namespace std;

// default constructor
Node::Node()
{
  // initialize all variables as null
  data = '\0'; // assign as null character
  next = NULL;
  left = NULL;
  right = NULL;
}

// regular constructor
Node::Node(char newdata)
{
  data = newdata;
  left = NULL;
  right = NULL;
  next = NULL;
}

// destructor, which destroys anything left on the heap
Node::~Node()
{
  left = NULL;
  right = NULL;
  next = NULL;
}

// returns next node pointer
Node* Node::getNext()
{
  return next;
}
		
// returns left child
Node* Node::getLeft()
{
  return left;
}

// returns right child
Node* Node::getRight()
{
  return right;
}

// returns data value
char Node::getValue()
{
  return data;
}

// set next pointer
void Node::setNext(Node* newnext)
{
  next = newnext;
}

// set left child
void Node::setLeft(Node* newleft)
{
  left = newleft;
}

// set right child
void Node::setRight(Node* newright)
{
  right = newright;
}

// set new data
void Node::setValue(char newdata)
{
  data = newdata;
}
