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

char* toPostfix(char input[100], int length);

int main()
{

  // prompt user for input
  int max = 100;
  char input[max];
  cout << "Please enter a mathematical expression. Rules:" << endl;
  cout << "\tThis program only parses single-digit integers." << endl;
  cout << "\tYou may use the +, -, /, *, ^ and () symbols." << endl;
  cin.getline(input, max);
  //cout << input << endl;

  int length = strlen(input);
  char* output = toPostfix(input, length);

  return 0;
}

/**
 * This function converts the user's mathematical expression into postfix
 * notation.
 */

char* toPostfix(char input[100], int length)
{
  // create the output string
  char* output = new char[length + 1];
  output[length + 1] = '\0';
  int outCount = 0; // this counts tracks we are in the output string
  
   for (int i = 0; i < length; i++)
    {
      // if operand, push to the stack
      if (input[i] >= '0' && input[i] <= '9')
        {
          // convert the value to an integer by subtracting character '0'
          //int operand = input[i] - '0';
          output[outCount] = input[i];
	  //cout << output[outCount] << endl;
	  outCount++;
        }
      else if (input[i] == '+'|| input[i] == '-')
	{
	  // peek at the stack
	  // if the thing on the stack has a higher precedence, output
	  // else add onto stack
	}
    }
   cout << output << endl;
   return NULL;
}

/**
 * push is one of the functions for the stack. It adds the newest value to the
 * "top" of the stack - in this case, the next node in the linked list.
 */
void push(Node* current, Node* newnode)
{
  // get to the end of the linked list
  while (current->getNext() != NULL)
    {
      current = current->getNext();
    }
  current->setNext(newnode);
}

void pop(Node* current, Node* previous)
{
  // get to the end of the linked list
  while (current->getNext() != NULL)
    {
      previous = current;
      current = current->getNext();
    }
  
}