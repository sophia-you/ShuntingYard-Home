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

char* toPostfix(char input[100], int length, Node* &stackHead, Node* &queueHead);
void push(Node* &head, Node* newnode);
char pop(Node* &head, Node* previous);
void enqueue(Node* &head, Node* newnode);
void dequeue(Node* &head);
Node* peek(Node* current);
void print(Node* current);
int precedence(char input);

int main()
{
  // head of the stack linked list
  Node* stackHead = NULL;

  // head of the queue linked list
  Node* queueHead = NULL;

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
      //cout << input << endl;

      if (strcmp(input, "quit") == 0)
	{
	  wantToQuit = true;
	}
      else
	{
      int length = strlen(input);
      char* output = toPostfix(input, length, stackHead, queueHead);
	}
    }
  return 0;
}

/**
 * This function converts the user's mathematical expression into postfix
 * notation.
 */

char* toPostfix(char input[100], int length, Node* &stackHead, Node* &queueHead)
{

  // walk through the input and convert to postfix
   for (int i = 0; i < length; i++)
    {
      // print(queueHead);
      // cout << "input: " << input[i] << endl;

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
	      // cout << "on stack: " << onStack << endl;
	    }
	  
	  // if the precedence of the current operator is lower
	  // for example, if the current operatur i is a '+' but
	  // the thing on the stack is a '*'
	  if (peek(stackHead) != NULL && precedence(input[i]) <= precedence(onStack))
	    {
	      // while the precedence is still <=, pop off the top of the stack
	      while (precedence(input[i]) <= precedence(onStack) &&
		     peek(stackHead) != NULL)
		{
		  onStack = peek(stackHead)->getValue();
		  // cout << "on stack: " << onStack << endl;
		  
		  // we should pop the current char on stack off
		  // cout << "stack operator has greater prevalence." << endl;

		  // the queue stores the values in postfix
		  Node* toQueue = new Node();
		  if (stackHead != NULL && precedence(input[i]) <= precedence(onStack))
		    {
		      // cout << "pop to the queue!" << endl;
		      toQueue->setValue(pop(stackHead, stackHead));
		      // cout << "to Queue: " << toQueue->getValue() << endl;
		      enqueue(queueHead, toQueue);

		    }
		}
	    }

	  // if it's first on the stack or has > precedence
	  else if (peek(stackHead) == NULL || precedence(input[i]) > precedence(onStack))
	    {
	      cout << "we precede" << endl;
	    }

	   Node* toStack = new Node();
           toStack->setValue(input[i]);
           push(stackHead, toStack);
	   cout << "on stack: " << peek(stackHead)->getValue() << endl;
	   
	}
      
      else if (input[i] == '(')
	{
	  cout << "open parentheses" << endl;
	  // push this open parenthesis onto the stack
	  // (we're going to throw it away later)
	  Node* toStack = new Node();
          toStack->setValue(input[i]);
          push(stackHead, toStack);
          cout << "on stack in parenthesis: " << peek(stackHead)->getValue() << endl;
	}
      else if (input[i] == ')')
	{
	  cout << "close parentheses: " << endl;
	  print(stackHead);
	  cout << "" << endl;
	  char onStack = '\0';
	  if (peek(stackHead) != NULL)
	    {
	      onStack = peek(stackHead)->getValue();
	    }

	  // pop everything up to the open parenthesis into output
	  while(peek(stackHead) != NULL && onStack != '(')
	    {
	      onStack = peek(stackHead)->getValue();
	      cout << "on stack in parenthesis: " << onStack << endl;
	      Node* toQueue = new Node();
	      toQueue->setValue(pop(stackHead, stackHead));
	      cout << "in parenthesis to queue: " << toQueue->getValue() << endl;
	      print(stackHead);

	      if (onStack != '(')
		{
		  enqueue(queueHead, toQueue);
		}
	      print(queueHead);
	    }
	  
	}
      }

   
   // pop everything from the stack into the output
   while (peek(stackHead) != NULL)
     {
       //output[outCount] = pop(stackHead, stackHead);
       //outCount++;

       cout << "peek value: " << peek(stackHead)->getValue() << endl;
       Node* poppedNode = new Node();
       char poppedChar = pop(stackHead, stackHead);
       cout << "popped char: " << poppedChar << endl;
       poppedNode->setValue(poppedChar);
       cout << "popped node: " << poppedNode->getValue() << endl;
       enqueue(queueHead, poppedNode); 
     }
   //cout << "output string: " << output << endl;
   print(queueHead);
   while (peek(queueHead) != NULL)
     {
       dequeue(queueHead);
     }
   return NULL;
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
char pop(Node* &head, Node* previous)
{
  cout << "inside pop" << endl;
  if (head != NULL)
    {
      print(head);
      Node* current = head;
      Node* previous = head;
      while (current->getNext() != NULL)
	{
	  previous = current;
	  current = current->getNext();
	  // move down the list
	}
      // we have reached the last node in the list
      if (current == head)
	{
	  // set head to null
	  head = NULL;
	  return current->getValue();
	}
      else
	{
	  previous->setNext(NULL);
	  cout << "current->getValue() = " << current->getValue() << endl;
	  return current->getValue();
	  delete current;
	}
    }
  return ' ';
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
void dequeue(Node* &head)
{
  Node* temp = head;
  head = head->getNext();
  delete temp;
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
  
  cout << "inside print" << endl;
  while(current != NULL)
    {
      //cout << "inside while loop" << endl;
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
