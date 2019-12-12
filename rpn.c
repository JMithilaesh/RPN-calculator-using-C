/* Fill in your Name and GNumber in the following two comment fields
 * Name:Mithilaesh Jayakumar
 * GNumber:G01206238
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "token.h"
#include "hash.h"

/* Local Function Declarations (that you need to finish implementing) */
static int read_file(char *filename, char *line);
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok);

/* Pre-Written Local Function Declarations */
static void print_header(char *filename, int step);
static void print_step_header(int step);
static void print_step_footer(Symtab *symtab, Stack_head *stack);
static void print_step_output(int val);

/* Defines the largest line that can be read from a file */
#define MAX_LINE_LEN 255

/* This has been written for you.
 * Main function to run your program (written for you).
 * 1) Opens the file using the passed in filename. (your function)
 * -- If the file is not found (ie. fopen returns NULL), then exit(-1);
 * 2) Reads one line from the file.
 * -- The programs will all only be one line in size.
 * -- That line may be up to MAX_LINE_LEN long.
 * 3) Closes the file.
 * 4) Calls token_read_line(line, strlen(line))
 * -- This parses the line and prepares the tokens to be ready to get.
 * 5) While there are tokens remaining to parse: token_has_next() != 0
 * -- a) Get the next token: token_get_next()
 * 6) Parse the token (your function)
 * 7) Print out all of the relevant information
 */
int rpn(Stack_head *stack, Symtab *symtab, char *filename) {
  int step = 0; /* Used to track the program steps */
  int ret = 0;
  char line[MAX_LINE_LEN];
  Token *tok = NULL;

  /* Complete the read_file function that is defined later in this file. */
  ret = read_file(filename, line);
  if(ret != 0) {
    printf("Error: Cannot Read File %s.  Exiting\n", filename);
    exit(-1);
  }

  /* Pass the line into the tokenizer to initialize that system */
  token_read_line(line, strlen(line));

  /* Prints out the nice program output header */
  print_header(filename, step);

  /* Iterate through all tokens */
  while(token_has_next()) {
    /* Begin the next step of execution and print out the step header */
    step++; /* Begin the next step of execution */
    print_step_header(step);

    /* Get the next token */
    tok = token_get_next();
    /* Complete the implementation of this function later in this file. */
    ret = parse_token(symtab, stack, tok);
    if(ret != 0) {
      printf("Critical Error in Parsing.  Exiting Program!\n");
      exit(-1);
    }

    /* Prints out the end of step information */
    print_step_footer(symtab, stack);
  }

  return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Local function to open a file or exit.
 * Follow the Directions in the Project Documentation for this Function
 * Open filename, read its contents (up to MAX_LINE_LEN) into line, then
 *   close the file and return 0.
 * On any file error, return -1.
 */
static int read_file(char *filename, char *line) 
	{
		/* Initialize a File pointer */
		FILE *fp = NULL;
		/* Open the File to read */
		fp = fopen(filename,"r");
		if(fp == NULL)
			{
				return -1;
			}
		else
			{
				/* Get the first line from the File */
				fgets(line,MAX_LINE_LEN,fp);
				/* Close the file */
				fclose(fp);
				return 0;
			}
	}

/* (IMPLEMENT THIS FUNCTION)
 * Parses the Token to implement the rpn calculator features
 * Follow the Directions in the Project Documentation for this Function
 * You may implement this how you like, but many small functions would be good!
 * If the token you are passed in is NULL, return -1.
 * If there are any memory errors, return -1.
 */
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok) 
	{
		int type = tok->type;
		/* Temporary token pointers */
		Token *top_element1 = NULL;
		Token *top_element2 = NULL;
		int ret;
		/* Branching between token types */ 
		switch(type)
			{
				case TYPE_ASSIGNMENT:
					{
						top_element1 = stack_pop(stack);
						top_element2 = stack_pop(stack);
						/* Put the token into hash table */
						ret = hash_put(symtab,top_element2->variable,top_element1->value);
						if(ret != 0)
							{
								return -1;
							}
						break;
					}
				case TYPE_OPERATOR:
					{
						top_element1 = stack_pop(stack);
						top_element2 = stack_pop(stack);
						/* Temporary symbol pointers */
						Symbol *s_t_e1 = NULL;
						Symbol *s_t_e2 = NULL;
						Token *output = NULL;
						/* Get the type of token operator */
						int operator = tok->oper;
						int result;
						switch(operator)
							{
								case OPERATOR_PLUS:
									{
										/* Add the token values directly if they are numbers */
										if((top_element1->type == TYPE_VALUE) && (top_element2->type == TYPE_VALUE))
											{
												result = top_element1->value + top_element2->value;
												/* Push the output back to stack as token*/
												output = token_create_value(result);
												ret = stack_push(stack,output);
												if(ret != 0)
													{
														return -1;
													}
											}
										else
											{
											/* Retrieve the symbols from hash table and add if tokens are variables */
												s_t_e1 = hash_get(symtab,top_element1->variable);
												s_t_e2 = hash_get(symtab,top_element2->variable);
												result = s_t_e1->val + s_t_e2->val;
												/* Push the output back to stack as token*/
												output = token_create_value(result);
												ret = stack_push(stack,output);
												if(ret != 0)
													{
														return -1;
													}
											}
										break;
									}
								case OPERATOR_MINUS:
									{
										/* Subtract the token values directly if they are numbers */
										if((top_element1->type == TYPE_VALUE) && (top_element2->type == TYPE_VALUE))
											{
												result = top_element2->value - top_element1->value;
												/* Push the output back to stack as token*/
												output = token_create_value(result);
												ret = stack_push(stack,output);
												if(ret != 0)
													{
														return -1;
													}
											}
										else
											{
											/* Retrieve the symbols from hash table and subtract if tokens are variables */
												s_t_e1 = hash_get(symtab,top_element1->variable);
												s_t_e2 = hash_get(symtab,top_element2->variable);
												result = s_t_e2->val - s_t_e1->val;
												/* Push the output back to stack as token*/
												output = token_create_value(result);
												ret = stack_push(stack,output);
												if(ret != 0)
													{
														return -1;
													}
											}
										break;
									}
								case OPERATOR_MULT:
									{
										/* Multiply the token values directly if they are numbers */
										if((top_element1->type == TYPE_VALUE) && (top_element2->type == TYPE_VALUE))
											{
												result = top_element2->value * top_element1->value;
												/* Push the output back to stack as token*/
												output = token_create_value(result);
												ret = stack_push(stack,output);
												if(ret != 0)
													{
														return -1;
													}
											}
										else
											{
											/* Retrieve the symbols from hash table and multiply if tokens are variables */
												s_t_e1 = hash_get(symtab,top_element1->variable);
												s_t_e2 = hash_get(symtab,top_element2->variable);
												result = s_t_e2->val * s_t_e1->val;
												/* Push the output back to stack as token*/
												output = token_create_value(result);
												ret = stack_push(stack,output);
												if(ret != 0)
													{
														return -1;
													}
											}
										break;
									}
								case OPERATOR_DIV:
									{
										/* Divide the token values directly if they are numbers */
										if((top_element1->type == TYPE_VALUE) && (top_element2->type == TYPE_VALUE))
											{
												result = top_element2->value / top_element1->value;
												/* Push the output back to stack as token*/
												output = token_create_value(result);
												ret = stack_push(stack,output);
												if(ret != 0)
													{
														return -1;
													}
											}
										else
											{
											/* Retrieve the symbols from hash table and divide if tokens are variables */
												s_t_e1 = hash_get(symtab,top_element1->variable);
												s_t_e2 = hash_get(symtab,top_element2->variable);
												result = s_t_e2->val / s_t_e1->val;
												/* Push the output back to stack as token*/
												output = token_create_value(result);
												ret = stack_push(stack,output);
												if(ret != 0)
													{
														return -1;
													}
											}
										break;
									}
								}
							/* Free the temporary symbol pointers */
							symbol_free(s_t_e1);
							symbol_free(s_t_e2);
						break;
					}
				case TYPE_VARIABLE:
					{
						/* Push the token to stack */
						ret = stack_push(stack,tok);
						if(ret != 0)
							{
								return -1;
							}
						break;
					}
				case TYPE_VALUE:
					{
						/* Push the token to stack */
						ret = stack_push(stack,tok);
						if(ret != 0)
							{
								return -1;
							}
						break;
					}
				case TYPE_PRINT:
					{
						/* Get the token from stack */
						top_element1 = stack_pop(stack);
						/* Check if the token is in hash table */
						Symbol *s_t_e = hash_get(symtab,top_element1->variable);
						if(s_t_e != NULL)
							{
								/* Print value of symbol if in hash table */
								print_step_output(s_t_e->val);
							}
						else
							{
								/* Print value of token itself */
								print_step_output(top_element1->value);
							}
						symbol_free(s_t_e);
					}
				}
			/* Free temporary token pointers */
			token_free(top_element1);
			token_free(top_element2);
		return 0;
	}

/* This has been written for you.
 * Prints out the main output header
 */
static void print_header(char *filename, int step) {
  printf("######### Beginning Program (%s) ###########\n", filename);
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step);
  token_print_remaining();
  printf("o-------------------\n");
}

/* This has been written for you.
 * Prints out the information at the top of each step
 */
static void print_step_header(int step) {
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step++);
}

/* This has been written for you.
 * Prints out the output value (print token) nicely
 */
static void print_step_output(int val) {
  printf("|-----Program Output\n");
  printf("| %d\n", val);
}

/* This has been written for you.
 * Prints out the information at the bottom of each step
 */
static void print_step_footer(Symtab *symtab, Stack_head *stack) {
  hash_print_symtab(symtab);
  stack_print(stack);
  token_print_remaining();
  printf("o-------------------\n");
}
