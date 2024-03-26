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

// FUNCTION PROTOTYPES

// initial conversion from user input
void toTree(char input[100], int length, Node* &stackHead, Node* &queueHead);
int precedence(char input);
void print(Node* current);

// dealing with the expression tree
void makeTree(Node* &queueHead, Node* &treeHead);
void printTree(Node* current, int numTabs);

// prefix, infix, and postfix stuff
/*
 * Note to self
 * Prefix | operator before operands; parent, left child, right child
 * Infix | operator between operands; left child, parent, right child
 * Postfix | operator after operands; left chid, right child, parent
 */
void toPrefix(Node* current, Node* &queueHead);
void toInfix(Node* current, Node* &queueHead);
void toPostfix(Node* current, Node* &queueHead);

// general functions
void push(Node* &head, Node* newnode);
Node* pop(Node* &head, Node* previous);
void enqueue(Node* &head, Node* newnode);
Node* dequeue(Node* &head);
Node* peek(Node* current);

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
	  toTree(input, length, stackHead, queueHead);

	  // make and print the expression tree
	  makeTree(queueHead, treeHead);
	  Node* current = treeHead;

	  // hypothetically, we shouldn't need this while loop,
	  // because when the postfix expression has been completely
	  // fed into the expression tree, the only thing on the tree
	  // stack should be the VERY LAST operator from the postfix.
	  // thus, we would only print out the left and right children
	  // of treeHead. This is a safeguard in case something
	  // very wrong happens.
	  while (current != NULL)
	    {
	      printTree(current, 0);
	      current = current->getNext();
	    }

	  // based on user input, convert expression tree to pre, in, or postfix
	  cout << "" << endl;
	  cout << "Please type, using all lowercase, one of the following commands to convert your expression tree." << endl;
	  cout << "\tprefix" << endl;
	  cout << "\tinfix" << endl;
	  cout << "\tpostfix" << endl;
	  cout << "" << endl;

	  cin.getline(input, max);

	  // convert to PREFIX
	  if (strcmp(input, "prefix") == 0)
	    {
	      cout << "Prefix Expression: ";
	      toPrefix(treeHead, queueHead);
	      print(queueHead);
	      cout << endl;
	    }
	  // convert to INFIX
	  else if (strcmp(input, "infix") == 0)
            {
	      cout << "Infix Expression: ";
	      toInfix(treeHead, queueHead);
              print(queueHead);
              cout << endl;
            }
	  // convert to POSTFIX
	  else if (strcmp(input, "postfix") == 0)
            {
	      cout << "Postfix Expression: ";
	      toPostfix(treeHead, queueHead);
              print(queueHead);
              cout << endl;
            }

	  /*
	   * now that we've finished converting the user input,
	   * we should take precautions to clear the stack, queue
	   * and tree heads - otherwise, some of that data may
	   * carry over the next time the user gives an input
	   */

	  while (queueHead != NULL)
	    {
	      Node* current = dequeue(queueHead);
	     
	    }
	  while (treeHead != NULL)
	    {
	      Node* current = dequeue(treeHead);
	      if (current != treeHead)
		{
		  Node* temp = current;
		  delete temp;
		}
	    }
	}
    }
  return 0;
}

/**
 * This function converts the user's mathematical expression into postfix
 * notation.
 */

void toTree(char input[100], int length, Node* &stackHead, Node* &queueHead)
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
		  if (stackHead != NULL && precedence(input[i]) <= precedence(onStack))
		    {
		      Node* toQueue = pop(stackHead, stackHead);
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
	      Node* toQueue = pop(stackHead, stackHead);

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
       Node* poppedNode = pop(stackHead, stackHead);
       enqueue(queueHead, poppedNode); 
     }

   cout << "Postfix Conversion: ";
   print(queueHead);
   cout << endl;
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

  Node* dequeued = dequeue(queueHead);
  dequeued->setNext(NULL); // make sure dequeued is NOT connected to anything

  // if the dequeued value is an OPERAND, push it onto the tree stack
  if (dequeued->getValue() >= '0' && dequeued->getValue() <= '9')
    {
      // push the dequeued value onto the tree stack                           
      push(treeHead, dequeued);
    }

  // otherwise, the dequeued value is an OPERATOR
  else
    {
      //cout << "dequeued is an operator" << endl;
      // pop off the last two things, they become the left and right children
      Node* right = pop(treeHead, treeHead);
      Node* left = pop(treeHead, treeHead);
      dequeued->setLeft(left);
      dequeued->setRight(right);
      push(treeHead, dequeued);
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
 * This function uses the expression tree to create a
 * prefix expression.
 */
void toPrefix(Node* current, Node* &queueHead)
{
  // once everything is done, jump out of the function                                 
  if (current == NULL)
    {
      return;
    }

  // operator before operands
  enqueue(queueHead, current);
  toPrefix(current->getLeft(), queueHead);
  toPrefix(current->getRight(), queueHead);
}

/**                                                                                    
 * This function uses the expression tree to create an
 * infix expression.
 */
void toInfix(Node* current, Node* &queueHead)
{
  // once everything is done, jump out of the function                                  
  if (current == NULL)
    {
      return;
    }

  // operator between operands                                     
  toInfix(current->getLeft(), queueHead);
  enqueue(queueHead, current);
  toInfix(current->getRight(), queueHead);
}

/**
 * This function uses the expression tree to create
 * a postfix expression.
 */
void toPostfix(Node* current, Node* &queueHead)
{
  // once everything is done, jump out of the function
  if (current == NULL)
    {
      return;
    }

  // operator after operands
  toPostfix(current->getLeft(), queueHead);
  toPostfix(current->getRight(), queueHead);
  enqueue(queueHead, current);
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
	}
      else
	{
	  previous->setNext(NULL);
	  return current;
	}
    }
  return NULL;
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
