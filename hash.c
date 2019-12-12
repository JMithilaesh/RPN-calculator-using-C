/* Fill in your Name and GNumber in the following two comment fields
 * Name:Mithilaesh Jayakumar
 * GNumber:G01206238
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "hash.h"

/* (IMPLEMENT THIS FUNCTION)
 * In this function, you will create a new Symtab struct.
 * Follow the Directions in the Project Documentation for this Function
 * Return the pointer to the new symtab.
 * On any memory errors, return NULL
 */
Symtab *hash_initialize() 
	{
		/* Create memory for symtab */
		Symtab *symtab = malloc(sizeof(Symtab));
		if(symtab != NULL)
		{
			/* Initialize the values in symtab */
			symtab->capacity = HASH_TABLE_INITIAL;
			symtab->size = 0;
			/* Create memory for symbol table */
			symtab->table = malloc(sizeof(Symbol));
			int index;
			/* Initialize all the indexes of symbol table to NULL */
			for(index = 0; index < HASH_TABLE_INITIAL; index++)
				{
					symtab->table[index] = NULL;
				}
			return symtab;
		}
		else
		{
			return NULL;
		}
	}

/* (IMPLEMENT THIS FUNCTION)
 * Destroy your Symbol Table.
 * Follow the Directions in the Project Documentation for this Function
 * Return on any memory errors.
 */
void hash_destroy(Symtab *symtab) 
	{
		int index;
		/* Temporary symbol pointers */
		Symbol *walker = NULL;
		Symbol *reaper = NULL;
		/* Iterate each index of symbol table */
		for(index = 0;index < symtab->capacity; index++)
			{
				walker = symtab->table[index];
				/* Iterate each list in that index */
				while(walker != NULL)
					{
						reaper = walker;
						walker = walker->next;
						/* Free the list */
						symbol_free(reaper);
					}
			}
		/* Free the symtab and the symbol table itself */
		free(symtab->table);
		free(symtab);
		return;
	}

/* (IMPLEMENT THIS FUNCTION)
 * Return the capacity of the table inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_capacity(Symtab *symtab) 
	{
		if(symtab != NULL)
			{
				/* Return the capacity */
				return symtab->capacity;
			}
		else
			{
				return -1;
			}
	}

/* (IMPLEMENT THIS FUNCTION)
 * Return the number of used indexes in the table (size) inside of symtab.
 * If symtab is NULL, return -1;
 */
int hash_get_size(Symtab *symtab) 
	{
		if(symtab != NULL)
			{
				/* Return the size */
				return symtab->size;
			}
		else
			{
				return -1;
			}
	}

/* (IMPLEMENT THIS FUNCTION)
 * Adds a new Symbol to the symtab via Hashing.
 * Follow the Directions in the Project Documentation for this Function
 * If symtab is NULL, there are any malloc errors, or if any rehash fails, return -1;
 * Otherwise, return 0;
 */
int hash_put(Symtab *symtab, char *var, int val)
	{
		if (symtab == NULL)
			{
				/* Check if symtab is NULL */
				return -1;
			}
		else
			{
				/* Find the index of given variable using hash code */
				long index = hash_code(var) % symtab->capacity;
				Symbol *walker = NULL;
				int already_exist = 0;
				/* Check if the symbol is already in hash table */
				walker = symtab->table[index];
					while (walker != NULL)
						{
							if (strcmp(walker->variable, var) == 0)
								{
									/* Update the value in hash table */
									walker->val = val;
									already_exist = 1;
								}
							walker = walker->next;
						}				
				if (already_exist == 0)
					{
						/* If not already present check the load for inserting */
						float load = symtab->size / symtab->capacity;
							if (load >= 2.0)
								{
									/* Rehash to twice the size if load exceeded */
									int new_capacity = 2 * (symtab->capacity);
									hash_rehash(symtab, new_capacity);
								}
							/* If within load insert to the index*/
							Symbol *new_symbol = symbol_create(var, val);
							Symbol *reaper = symtab->table[index];
							/* If no list is at the index then insert as first*/
							if (symtab->table[index] == NULL)
								{
									symtab->table[index] = new_symbol;
								}
							else
							/* If already list there insert at the end */
								{
									while (reaper->next != NULL)
										{
											reaper = reaper->next;
										}
									reaper->next = new_symbol;
								}
						symtab->size++;
					}
				return 0;
			}
	}	

/* (IMPLEMENT THIS FUNCTION)
 * Gets the Symbol for a variable in the Hash Table.
 * Follow the Directions in the Project Documentation for this Function
 * On any NULL symtab or memory errors, return NULL
 */
Symbol *hash_get(Symtab *symtab, char *var) 
	{
		/*Check if symtab is NULL */
		if (symtab == NULL)
			{
				return NULL;
			}
		/* Compute index for given variable using hash code */
		long index = hash_code(var) % symtab->capacity;
		Symbol *walker = symtab->table[index];
		Symbol *symbol = NULL;
		/* Traverse through list at that index */
		while (walker != NULL)
			{
				if (strcmp(walker->variable, var) == 0)
					{
						/* Return the matching symbol for the passed variable */
						symbol = symbol_copy(walker);
						return symbol;
					}
				walker = walker->next;
			}
		/* If the symbol for passed variable not present */
		if (walker == NULL)
			{
				return NULL;
			}
		return NULL;
	}

/* (IMPLEMENT THIS FUNCTION)
 * Doubles the size of the Array in symtab and rehashes.
 * Follow the Directions in the Project Documentation for this Function
 * If there were any memory errors, set symtab->array to NULL
 * If symtab is NULL, return immediately.
 */
void hash_rehash(Symtab *symtab, int new_capacity) 
	{
		/* Dynamic memory creation for new symbol table */
		Symbol **new_table = malloc(sizeof(Symbol)*new_capacity);
		Symbol *walker = NULL;
		Symbol *reaper = NULL;
		int index = 0;
		/* Assigning all the indexes of new table to NULL */
		for (index = 0; index < new_capacity; index++)
			{
				new_table[index] = NULL;
			}
		/* Traversing the indexes of old symbol table */
		for (index = 0; index < symtab->capacity; index++)
			{
				if (symtab->table[index] != NULL)
					{
						walker = symtab->table[index];
						/* Traversing the linked list at each index of old table */
						while (walker != NULL)
							{
								/* Compute index using hash code for new capacity */
								long new_index = hash_code(walker->variable) % new_capacity;
								/* Create new symbol to insert at that index */
								Symbol *new_symbol = malloc(sizeof(Symbol));
								/* Copy the values from old symbol to new symbol */
								new_symbol = symbol_create(walker->variable, walker->val);
								/* Insert first if no list present at that index */
								if (new_table[new_index] == NULL)
									{
										new_table[new_index] = new_symbol;
									}
								else
									{
										/* Insert last if any list already present at the index */
										reaper = new_table[new_index];
										while (reaper->next != NULL)
											{
												reaper = reaper->next;
											}
										reaper->next = new_symbol;
									}
								walker = walker->next;
							}
					}
			}
		/* Point the old symtab to new symbol table */
		symtab->table = new_table;
		/* Update the new capacity */	
		symtab->capacity = new_capacity;
	}

/* Implemented for you.
 * Provided function to print the symbol table */
void hash_print_symtab(Symtab *symtab) {
  if(symtab == NULL) {
    return;
  }
  printf("|-----Symbol Table [%d size/%d cap]\n", symtab->size, symtab->capacity);

  int i = 0;
  Symbol *walker = NULL;

  /* Iterate every index, looking for symbols to print */
  for(i = 0; i < symtab->capacity; i++) {
    walker = symtab->table[i];
    /* For each found linked list, print every symbol therein */
    while(walker != NULL) {
      printf("| %10s: %d \n", walker->variable, walker->val);
      walker = walker->next;
    }
  }
  return;
}

/* This function is written for you.
 * This computes the hash function for a String
 */
long hash_code(char *var) {
  long code = 0;
  int i;
  int size = strlen(var);

  for(i = 0; i < size; i++) {
    code = (code + var[i]);
    if(size == 1 || i < (size - 1)) {
      code *= 128;
    }
  }

  return code;
}
