/*
 * File: symbol.c
 * Contains functions to handle the symbol table
 */
#include "lc3asm.h"

/*
 * The symbol table is implemented as a linked list, with each node as the "symbol" structure
 */

/*
 * 
 * name: append_symtable
 * Add a new symbol entry into the symbol table (the linked list).
 * No duplicate is inserted; the function firstpass() generates an error if a duplicate label is found
 * 
 */
void append_symtable(symbol **symtable, char *label, int addr)
{
    if (*symtable == NULL)
    {
        *symtable = (symbol *)malloc(sizeof(symbol));
        if (*symtable == NULL)
        {
            return;
        }
        
        strcpy((*symtable)->label, label);
        (*symtable)->addr = addr;
        (*symtable)->next = NULL;
    }
    else
    {
        append_symtable(&((*symtable)->next), label, addr);
    }
}

/*
 * 
 * name: search_table
 * Search the table for a given label and return the associated address
 * 
 */
int search_table(symbol *symtable, char *label)
{
    if (symtable == NULL)
    {
        return -1;
    }
    
    if (strcmp(symtable->label, label) == 0)
    {
        return symtable->addr;
    }
    else
    {
        return search_table(symtable->next, label);
    }
}

/*
 * 
 * name: write_symfile
 * Writes the symbol table to the .sym output file pointed to by a member of the files structure
 * 
 */
void write_symfile(symbol *symtable, files *f)
{
    fprintf(f->symfile, "-------------------------------------------\n");
    fprintf(f->symfile, "       SYMBOL        |       ADDRESS       \n");
    fprintf(f->symfile, "-------------------------------------------\n");
    symbol *n;
    n = symtable;
    while (n != NULL)
    {
        fprintf(f->symfile, "%-21s x%X\n", n->label, n->addr);
        n = n->next;
    }
    fprintf(f->symfile, "-------------------------------------------\n");
}

/*
 * 
 * name: destroy_symtable
 * Frees memory associated with the symbol table linked list pointed to by "symtable"
 * 
 */
void destroy_symtable(symbol *symtable)
{
    if (symtable == NULL)
    {
        return;
    }
    
    symbol *prev, *curr, *tmp;
    prev = curr = symtable;
    while (curr->next != NULL)
    {
        prev = curr;
        curr = curr->next;
    }
    tmp = curr;
    free(curr);
    if (tmp == symtable)
    {
        symtable = NULL;
    }
    else
    {
        prev->next = NULL;
        destroy_symtable(symtable);
    }
}
