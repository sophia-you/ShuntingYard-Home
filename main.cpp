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
Node* peek(Node* current);
void print(Node* current);
int precedence(char input);

int main()
{
  // head of the stack linked list
  Node* stackHead = new Node();

  // head of the queue linked list
  Node* queueHead = new Node();

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
      char* output = toPostfix(input, length, stackHead);
	}
    }
  return 0;
}

/**
 * This function converts the user's mathematical expression into postfix
 * notation.
 */

char* toPostfix(char input[100], int length, Node* &head)
{
  // create the output string
  char* output = new char[length + 1];
  output[length + 1] = '\0';
  int outCount = 0; // this counts tracks we are in the output string

   for (int i = 0; i < length; i++)
    {
      cout << "input: " << input[i] << endl;
      // if operand, push to output
      if (input[i] >= '0' && input[i] <= '9')
        {
          // convert the value to an integer by subtracting character '0'
          //int operand = input[i] - '0';
          output[outCount] = input[i];
	  //cout << output[outCount] << endl;
	  outCount++;
	  //cout << "hi" << endl;
        }

      // operator
      if (input[i] == '+' ||
               input[i] == '-' ||
               input[i] == '*' ||
               input[i] == '/' ||
               input[i] == '^')
	{
	  char onStack = peek(stackHead)->getValue();

	  // if the precedence of the current operator is lower
	  if (precedence(input[i]) < precedence(onStack))
	    {
	      while (precedence(input[i]) < precedence(onStack))
		{
	      // we should pop the current char on stack off
	      cout << "stack operator has greater prevalence." << endl;
              // pop the stack thing to the output queue
              //output[outCount] = pop(stackHead, stackHead);
              //outCount++;

	      Node* toQueue = new Node();
	      toQueue->setValue(pop(stackHead, stackHead));
	      enqueue(queueHead, toQueue);
	      onStack = peek(stackHead)->getValue();
		}
	    }
	  else if (precedence(input[i]) >= precedence(onStack))
	    {
	      cout << "we precede" << endl;
	    }

	   Node* toStack = new Node();
           toStack->setValue(input[i]);
           push(stackHead, toStack);
	   
	}
      
      else if (input[i] == '(')
	{
	  cout << "open parentheses" << endl;
	}
      else if (input[i] == ')')
	{
	  cout << "close parentheses" << endl;
	}
      // this boolean tests whether the character on the stack has higher
      // precedence
      /*bool supercedes = false;

      // operator
      if (input[i] == '+' ||
	       input[i] == '-' ||
	       input[i] == '*' ||
	       input[i] == '/' ||
	       input[i] == '^')
	{
	  // this boolean tests whether the character on the stack has higher
	  // precedence
	  //bool supercedes = false;
	  char onStack = peek(head)->getValue();

	  // everything except the - operation supercedes +
	  if (input[i] == '+')
	    {
	      // anything other than the minus sign has greater precedence
	      if (onStack != '\0' && onStack != '-')
		{
		  // onStack supercedes
		  supercedes = true;
		  
		}

	    }
	  
	    if (input[i] == '-')
            {
              // anything other than the plus sign has greater precedence
              if (onStack != '\0' && onStack != '+')
                {
                  // onStack supercedes
                  supercedes = true;

                }

            }
	  
	  else if (input[i] == '*' || input[i] == '/')
            {
              if (onStack == '^')
                {
                  // the stack operator has greater precedence
		  supercedes = true;
                }

	    }
	
	  if (supercedes)
	    {
	      cout << "stack operator has greater prevalence." << endl;
              // pop the stack thing to the output queue
              output[outCount] = pop(head, head);
              outCount++;

	    }
	  if (!supercedes) // the current operation has = or > precedence
	    {
	      cout << "current operator has greater prevalence." << endl;
	    }

	  // push the current operator to the stack
	   Node* toStack = new Node();
           toStack->setValue(input[i]);
           push(head, toStack);
	  
	  // peek at the stack
	  // if the thing on the stack has a higher precedence, output
	  // else add onto stack
	}
      print(head);
      // if the current operator is a left parentheses
      if (input[i] == '(')
	{
	  Node* toStack = new Node();
	  toStack->setValue(input[i]);
	  cout << toStack << endl;
	  push(head, toStack);
	}
      
      // @ a right parenthesis, we must pop everything off the stack
      else if (input[i] == ')')
        {
	  cout << "right parenthesis reached" << endl;
	  print(head);
	  // while the last thing on the stack is NOT a left parentheses
	  while (peek(head)->getValue() != '(' || peek(head) != head)
	    {
	      // add the last thing on stack to the output
	      output[outCount] = pop(head, head);
	      outCount++;
	    }
	  if (peek(head)->getValue() == '(')
	    {
	      // we've reached the end of the parenthetical expression
	      if (peek(head) != head)
		{
		  delete peek(head);
		}
	      else
		{
		  peek(head)->setValue('\0');
		  peek(head)->setNext(NULL);
		}
	    }
	}
      // put it on the stack immediately
      // if the current operator is a right parentheses
      // while thing on the stack is NOT a left parentheses
      // pop off
      // when you get to the left parentheses, delete it off the stack
      cout << output << endl;
      */}

      
   // pop everything from the stack into the output
   while (peek(head) != head)
     {
       output[outCount] = pop(head, head);
       outCount++;
     }
   cout << output << endl;
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
  cout << peek(head)->getValue() << endl;
  Node* current = head;
  // get to the end of the linked list
  while (current->getNext() != NULL)
    {
      previous = current;
      current = current->getNext();
      //cout << "currently: " << current->getValue() << endl;
    }

  char toOutput = current->getValue(); // store this value

  // the stack is empty
  if (previous == current)
    {
      cout << "we're at the head rn" << endl;
      // we're at the head
      head->setValue('\0');
      head->setNext(NULL);
    }
  else
    {
      delete current; // remove from stack
      previous->setNext(NULL);
    }
  
  return toOutput;
  
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
      cout << current->getValue() << endl;
      current = current->getNext();
    }
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
      return 4;
    }
  return 0;
}
