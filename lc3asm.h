/* 
 * Header file for the LC-3 Assembler
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

//CONSTANTS
#define LINE_LEN 200
#define TOK_LEN 100

//STRUCTURES

//structure to hold the input and output file pointers along with filenames
//which acts as a convenient container for files to passed to functions
typedef struct files_t
{
    char infilename[100], outhexfilename[100], outbinfilename[100], symfilename[100];
    //source assembly code
    FILE *in;
    //output file containing the LC-3 ISA instructions in Hexadecimal
    FILE *outhex;
    //output file containing the LC-3 ISA instructions in Binary
    FILE *outbin;
    //output file containing the symbol table
    FILE *symfile;
} files;

//structure holding the details of one line of the source file
typedef struct line_t
{
    //the line read from source file
    char raw_line[LINE_LEN];
    //string without comments
    char processed_line[LINE_LEN];
    //array of strings containing individual elements of an instruction (label, opcode, operands)
    char tokens[10][TOK_LEN];
} line;

//structure for one node of the symbol table
typedef struct symbol_t
{
    //the label
    char label[TOK_LEN];
    //the address of the label
    unsigned int addr;
    //pointer to next node
    struct symbol_t *next;
} symbol;


//FUNCTION PROTOTYPES

//parse.c
void readline(line *l, files *f);
void restore_escaped_chars(char *raw);
void parsetokens(line *l);
int islabel(char *token);
void scannumber(char *token, int *n);

//symbol.c
void append_symtable(symbol **symtable, char *label, int addr);
int search_table(symbol *symtable, char *label);
void write_symfile(symbol *symtable, files *f);
void destroy_symtable(symbol *symtable);

//instruction.c
char *regcode(char *token);
char *int_to_bin(int val, char *buff, int sz);
void instruction_bintohex(char *inst_bin, char *inst_hex);

//generate_instruction.c
int generate_instruction(char inst[16], line *l, int opindex, symbol *symtable, int *LC);
int handle_BLKW(files *f, line *l, int opindex, int *LC);
int handle_STRINGZ(files *f, line *l, int opindex, int *LC);

//assemble.c
void assemble(files *f);
void firstpass(files *f, symbol **symtable, int *success);
void secondpass(files *f, symbol **symtable, int *success);
