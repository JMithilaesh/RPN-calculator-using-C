/* Fill in your Name and GNumber in the following two comment fields
 * Name:Mithilaesh Jayakumar
 * GNumber:G01206238
 */
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "stack.h"

/* (IMPLEMENT THIS FUNCTION)
 * Create a new Stack_head struct on the Heap and return a pointer to it.
 * Follow the Directions in the Project Documentation for this Function
 * On any malloc errors, return NULL
 */
Stack_head *stack_initialize() 
	{
		/* Dynamic allocation of memory for stack head */
		Stack_head *Head = malloc(sizeof(Stack_head));
		/* Initializing the values in stack head */
		if(Head != NULL)
			{
				Head->count = 0;
				Head->top = NULL;
				return Head;
			}
		else
			/* Return NULL if stack head not created */
			{
				return NULL;
			}
	}

/* (IMPLEMENT THIS FUNCTION)
 * Destroy a stack.
 * Follow the Directions in the Project Documentation for this Function
 * If head is NULL, you can just return.
 */
void stack_destroy(Stack_head *head) 
	{
		/* Variables to traverse through stack */
		Node *walker = head->top;
		Node *current = NULL;
		/* Traversing through the stack one by one */
		while(walker != NULL)
			{
				current = walker;
				walker = walker->next;
				/* Free token inside the current stack element */
				token_free(current->tok);
				if(current->tok != NULL)
					{
						return -1;
					}
				/* Free the current stack element */
				free(current);
				if(current != NULL)
					{
						return -1;
					}
			}
		/* Free the stack head */
		free(head);
		if(head != NULL)
			{
				return -1;
			}
	}

/* (IMPLEMENT THIS FUNCTION)
 * Push a new Token on to the Stack.
 * Follow the Directions in the Project Documentation for this Function
 * On any malloc errors, return -1.
 * If there are no errors, return 0.
 */
int stack_push(Stack_head *stack, Token *tok) 
	{
		/* Create a new node with the values in the token */
		Node *new_node = node_create(tok);
		if(new_node != NULL)
			{
				/* Insert the new node if it is the first element into the stack */
				if(stack->top == NULL)
					{
						stack->top = new_node;
						(stack->count)++;
					}
				else
					/* Insert the new node if already there are elements in stack */
					{
						new_node->next = stack->top;
						stack->top = new_node;
						(stack->count)++;
					}
				return 0;
			}
		else
			{
				return -1;
			}
	}

/* (IMPLEMENT THIS FUNCTION)
 * Pop a Token off of the Stack.
 * Follow the Directions in the Project Documentation for this Function
 * If the stack was empty, return NULL.
 */
Token *stack_pop(Stack_head *stack) 
	{
		/* Check if stack is not empty */
		if(stack != NULL && stack->top != NULL)
			{
				Token *node_tok = NULL;
				Node *walker = NULL;
				/* Get the token of top element in stack */
				node_tok = stack->top->tok;
				walker = stack->top;
				stack->top = stack->top->next;
				(stack->count)--;
				/* Free the top element after getting its token */
				node_free(walker);
				walker = NULL;
				if(walker != NULL)
					{
						return NULL;
					}
				return node_tok;
			}
		else
			{
				return NULL;
			}
	}

/* (IMPLEMENT THIS FUNCTION)
 * Return the token in the stack node on the top of the stack
 * Follow the Directions in the Project Documentation for this Function
 * If the stack is NULL, return NULL.
 * If the stack is empty, return NULL.
 */
Token *stack_peek(Stack_head *stack) 
	{
		/* Check if the stack is not empty */
		if(stack != NULL && stack->top != NULL)
			{
				Node *walker = NULL;
				walker = stack->top;
				Token *peek_tok = NULL;
				/* Get the token of the top element in stack */
				peek_tok = walker->tok;
				/* Free the temporary pointer */
				free(walker);
				return peek_tok;
			}
		else
			{
				return NULL;
			}
	}

/* (IMPLEMENT THIS FUNCTION)
 * Return the number of nodes in the stack.
 * Follow the Directions in the Project Documentation for this Function
 * If stack is NULL, return -1.
 * Return 1 if the stack is empty or 0 otherwise.
 */
int stack_is_empty(Stack_head *stack) 
	{
		/* Check if the stack is empty or not */
		if(stack == NULL || stack->top == NULL)
			{
				return 1;
			}
		else
			{
				return 0;
			}
	}

/* These two functions are written for you.
 * It recurses the stack and prints out the tokens in reverse order
 * eg. top->2->4->1->8 will print at Stack: 8 1 4 2
 * eg. stack_push(5) will then print Stack: 8 1 4 2 5
 */

/* This is implemented for you.
 * Recursive print. (Local function)
 * Base Case: node == NULL, return
 * Recursive Case: call print_node(node->next, print_data), then print node.
 */
static void print_node(Node *node) {
  if(node == NULL) {
    return;
  }
  token_print(node->tok);
  print_node(node->next);
  return;
}

/* This is implemented for you.
 * Setup function for the recursive calls.  Starts printing with stack->top
 */
void stack_print(Stack_head *stack) {
  if(stack == NULL) {
    return;
  }
  printf("|-----Program Stack\n");
  printf("| ");
  print_node(stack->top);
  printf("\n");
  return;
}
