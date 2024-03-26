#include <iostream>
#include <cstring>
#include "node.h" // includes binary tree + linked list node features

using namespace std;

/*
 * Author | Sophia You
 * Date | 03/05/2024
 * Description | This program executes a "shunting yard algorithm."
 * The user enters a basic mathematical expression (+ - * / ^) with single-
 * digit integers and parentheses and translates it into postfix notation.
 * Afterward, it visualizes the postfix expression as a tree. The user can ask
 * to output their expression as infix, prefix, or postfix notation.
 */

void toPostfix(char input[100], int length, Node* &stackHead, Node* &queueHead);
void makeTree(Node* &queueHead, Node* &treeHead);
void printTree(Node* current, int numTabs);
void push(Node* &head, Node* newnode);
Node* pop(Node* &head, Node* previous);
void enqueue(Node* &head, Node* newnode);
Node* dequeue(Node* &head);
Node* peek(Node* current);
void print(Node* current);
int precedence(char input);

int main()
{
  // head of the stack linked list
  Node* stackHead = NULL;

  // head of the queue linked list
  Node* queueHead = NULL;

  // head of the binary expresison tree
  Node* treeHead = NULL;
  
  bool wantToQuit = false;

  while(!wantToQuit)
    {
      // prompt user for input
      int max = 100;
      char input[max];
      cout << "Please enter a mathematical expression. Rules:" << endl;
      cout << "\tThis program only parses single-digit integers." << endl;
      cout << "\tYou may use the +, -, /, *, ^ and () symbols." << endl;
      cin.getline(input, max);

      if (strcmp(input, "quit") == 0)
	{
	  wantToQuit = true;
	}
      else
	{
	  // convert user input to postfix notation
	  int length = strlen(input);
	  toPostfix(input, length, stackHead, queueHead);

	  // make and print the expression tree
	  makeTree(queueHead, treeHead);
	  Node* current = treeHead;
	  while (current != NULL)
	    {
	      // we want to print out every tree in the stack
	      printTree(current, 0);
	      current = current->getNext();
	    }
	  // create the binary expression tree
	}
    }
  return 0;
}

/**
 * This function converts the user's mathematical expression into postfix
 * notation.
 */

void toPostfix(char input[100], int length, Node* &stackHead, Node* &queueHead)
{

  // walk through the input and convert to postfix
   for (int i = 0; i < length; i++)
    {

      // if number, push to output
      if (input[i] >= '0' && input[i] <= '9')
        {
         
	  Node* toQueue = new Node();
          toQueue->setValue(input[i]);
          enqueue(queueHead, toQueue);
        }

      // operator
      if (input[i] == '+' ||
               input[i] == '-' ||
               input[i] == '*' ||
               input[i] == '/' ||
               input[i] == '^')
	{

	  // this character keeps track of what is last on the stack
	  char onStack = '\0';
	  
	  if (peek(stackHead) != NULL)
	    {
	      onStack = peek(stackHead)->getValue();
	    }
	  
	  // if the precedence of the current operator is lower
	  // for example, if the current operator i is a '+' but
	  // the thing on the stack is a '*'
	  if (peek(stackHead) != NULL && precedence(input[i]) <= precedence(onStack))
	    {
	      // while the precedence is still <=, pop off the top of the stack
	      while (precedence(input[i]) <= precedence(onStack) &&
		     peek(stackHead) != NULL)
		{
		  onStack = peek(stackHead)->getValue();
		  
		  // the queue stores the values in postfix
		  Node* toQueue = new Node();
		  if (stackHead != NULL && precedence(input[i]) <= precedence(onStack))
		    {
		      toQueue = pop(stackHead, stackHead);
		      // toQueue->setValue(pop(stackHead, stackHead));
		      enqueue(queueHead, toQueue);
		    }
		}
	    }

	  // regardless of precedence, we must add the current operator
	  // to the stack.
	  Node* toStack = new Node();
          toStack->setValue(input[i]);
          push(stackHead, toStack);
	   
	}
      
      else if (input[i] == '(')
	{
	  Node* toStack = new Node();
          toStack->setValue(input[i]);
          push(stackHead, toStack);
	}

      // the idea here is that when we see a close parenthesis,
      // we want to pop everything currently on the stack OFF
      // and into the queue. When we reach an open parenthesis
      // we're going to pop it off too, but we will not
      // put it inside the queue. This is because the postfix
      // notation will not deal with parentheses.
      
      else if (input[i] == ')')
	{
	  char onStack = '\0';
	  if (peek(stackHead) != NULL)
	    {
	      onStack = peek(stackHead)->getValue();
	    }

	  // pop everything up to the open parenthesis into output
	  while(peek(stackHead) != NULL && onStack != '(')
	    {
	      onStack = peek(stackHead)->getValue();
	      Node* toQueue = pop(stackHead, stackHead);//new Node();
	      //toQueue->setValue(pop(stackHead, stackHead));

	      if (onStack != '(')
		{
		  enqueue(queueHead, toQueue);
		}
	    }
	  
	}
      }

   
   // pop everything from the stack into the output
   while (peek(stackHead) != NULL)
     {
       Node* poppedNode = pop(stackHead, stackHead);//new Node();
       //char poppedChar = pop(stackHead, stackHead);
       //poppedNode->setValue(poppedChar);
       enqueue(queueHead, poppedNode); 
     }

   cout << "Postfix Conversion: ";
   print(queueHead);
   cout << endl;

   // clear the queue
   /* while (peek(queueHead) != NULL)
     {
       dequeue(queueHead);
       }*/
}


/**
 * This function will make the binary expression
 * tree given a queue of values in postfix notation.
 */
void makeTree(Node* &queueHead, Node* &treeHead)
{
  // if we empty the queue, get out of makeTree
  if (queueHead == NULL)
    {
      return;
    }

  cout << "we are inside the make tree function!" << endl;
  Node* dequeued = dequeue(queueHead);
  cout << "we have dequeued: " <<  dequeued->getValue() << endl;
  dequeued->setNext(NULL); // make sure dequeued is NOT connected to anything

  // if the dequeued value is an OPERAND, push it onto the tree stack
  if (dequeued->getValue() >= '0' && dequeued->getValue() <= '9')
    {
      cout << "dequeued is an operand" << endl;
      // push the dequeued value onto the tree stack                           
      push(treeHead, dequeued);
      cout << "printed tree stack: ";
      print(treeHead);
      cout << endl;
    }

  // otherwise, the dequeued value is an OPERATOR
  else
    {
      cout << "dequeued is an operator" << endl;
      // pop off the last two things, they become the left and right children
      Node* right = pop(treeHead, treeHead);//new Node();
      //right->setValue(pop(treeHead, treeHead));
      Node* left = pop(treeHead, treeHead);//new Node();
      //left->setValue(pop(treeHead, treeHead));
      dequeued->setLeft(left);
      dequeued->setRight(right);
      cout << "\tleft child: " << dequeued->getLeft()->getValue() << endl;
      cout << "\tright child: " << dequeued->getRight()->getValue() << endl;
      push(treeHead, dequeued);
      cout << "printed tree stack: ";
      print(treeHead);
      cout << endl;
    }
  
  // call makeTree again; since we called on dequeued, queueHead has changed
  makeTree(queueHead, treeHead);
  
  // walk through the queue and dequeue the value
  // this means we will take out the thing at the front of the queue
  // take the dequeued node
  // if it is an operand, push it onto the tree STACK
  // if it is an operator
  // pop off the last two things on the stack and store them as L + R
  // push the operator onto the stack, set the L and R as the operandz
}

void printTree(Node* current, int numTabs)
{
  // if we get to a leaf, we want to jump out of print
  if (current == NULL)
    {
      return; // nothing to print
    }

  // otherwise, we have not reached a leaf
  numTabs += 1; // indent one more in
  printTree(current->getRight(), numTabs); // recursively print right child
  cout << endl; // line break
  for (int i = 1; i < numTabs; i++)
    {
      // indent the number of times as stated by numTabs
      cout << "\t";
    }
  cout << current->getValue() << "\n"; // print the current value
  printTree(current->getLeft(), numTabs); // recursively print left child
}

/**
 * push is one of the functions for the stack. It adds the newest value to the
 * "top" of the stack - in this case, the next node in the linked list.
 */
void push(Node* &head, Node* newnode)
{
  Node* current = head;
  if (head == NULL)
    {
      head = newnode;
      return;
    }
  
  // get to the end of the linked list
  while (current->getNext() != NULL)
    {
      current = current->getNext();
    }
  current->setNext(newnode);
}

/**
 * This function removes the item on the top of the stack
 * and returns that item to main. It deletes the item from the stack.
 */
Node* pop(Node* &head, Node* previous)
{
  //  cout << "inside pop" << endl;
  if (head != NULL)
    {
      Node* current = head;
      Node* previous = head;
      while (current->getNext() != NULL)
	{
	  previous = current;
	  current = current->getNext();	  // move down the list
	}
      
      // we have reached the last node in the list
      if (current == head)
	{
	  // set head to null
	  head = NULL;
	  return current;
	  //return current->getValue();
	}
      else
	{
	  previous->setNext(NULL);
	  return current;
	  //return current->getValue();
	  // delete current;
	}
    }
  return NULL;
  //return ' ';
}

/**
 * This function adds an item to the rear of the queue
 */
void enqueue(Node* &head, Node* newnode)
{
  Node* current = head;
  if (head == NULL)
    {
      head = newnode;
      return;
    }
  while (current->getNext() != NULL)
    {
      current = current->getNext();
    }
  
  current->setNext(newnode);
}

/**
 * This function removes the item at the front of the queue.
 */ 
Node* dequeue(Node* &head)
{
  Node* temp = head;
  head = head->getNext();
  return temp;
}

/**
 * This function looks at the first thing in the stack and returns its value
 */
Node* peek(Node* current)
{
  // current will be NULL if head is null (the list is empty)
  if (current == NULL)
    {
      return NULL;
    }
  while (current->getNext() != NULL)
    {
      current = current->getNext();
    }
  return current;
}

void print(Node* current)
{
  while(current != NULL)
    {
      if (current->getValue() != '\0')
	{
	  cout << current->getValue();
	  current = current->getNext();
	}
      else
	{
	  cout << "for some reason, head is null" << endl;
	  current = NULL;
	}
    }
  cout << endl;
}

/**
 * This function returns the degree of prevalence of each operator.
 */
int precedence(char input)
{
  if (input == '+' || input == '-')
    {
      return 1;
    }
  else if (input == '*' || input == '/')
    {
      return 2;

    }
  else if (input == '^')
    {
      return 3;
    }
  else if (input == '(' || input == ')')
    {
      return -1;
    }
  return 0;
}
