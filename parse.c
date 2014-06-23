/*
 * File: parse.c
 * contains functions to read and parse input from a source file
 */
#include "lc3asm.h"

/*
 * 
 * name: readline
 * @param: pointer to line structure, pointer to files structure
 * @output: reads a line from the source file into the lines structure. Removes comments and invokes
 *          restore_escaped_chars() to process the read line
 * 
 */
void readline(line *l, files *f)
{
    char str[LINE_LEN];
    fgets(l->raw_line, LINE_LEN, f->in);
    int numchars = strcspn(l->raw_line, ";\n");
    strncpy(str, l->raw_line, numchars);
    str[numchars] = '\0';
    restore_escaped_chars(str);
    strcpy(l->processed_line, str);
}

/*
 * 
 * name: restore_escaped_chars
 * When a line is read from the source file, the text containing an escape sequence is not interpreted as an escape
 * sequence because the '\' is escaped. So, for example, "\n" becomes "\\n". This function finds and removes the extra
 * escape sequence.
 * 
 * @param: a string containing a line read from the source file (without comments)
 * @output: the string is modified to replace escaped escape sequences with the escape sequence
 * 
 */
void restore_escaped_chars(char *raw)
{
    int numchars, length, bytestomove;
    char *p = strchr(raw, '\\');
    while (p != NULL)
    {
        numchars = strcspn(raw, "\\");
        //handle escape sequences
        if (*(p+1) == 'a')
        {
            *p = '\a';
        }
        else if (*(p+1) == 'b')
        {
            *p = '\b';
        }
        else if (*(p+1) == 'f')
        {
            *p = '\f';
        }
        else if (*(p+1) == 'n')
        {
            *p = '\n';
        }
        else if (*(p+1) == 'r')
        {
            *p = '\r';
        }
        else if (*(p+1) == 't')
        {
            *p = '\t';
        }
        else if (*(p+1) == 'v')
        {
            *p = '\v';
        }
        else if (*(p+1) == '\\')
        {
            *p = '\\';
        }
        else if (*(p+1) == '?')
        {
            *p = '\?';
        }
        else if (*(p+1) == '\'')
        {
            *p = '\'';
        }
        else if (*(p+1) == '\"')
        {
            *p = '\"';
        }
        else
        {
            //not a valid escape sequence. Break out.
            break;
        }
        length = strlen(raw);
        bytestomove = length - numchars;
        memmove(p+1, p+2, bytestomove);
        p = strchr(raw, '\\');
    }
}

/*
 * 
 * name: parsetokens
 * This function breaks the line read from the source file into tokens - which are components of the instruction
 * the .STRINGZ assembler directive is handled specially since we don't want the string argument to .STRINGZ to get
 * broken into tokens
 * 
 * @param: a pointer to the line structure
 * @output: the tokens array of the line structure initialized to components of the instruction
 * 
 */
void parsetokens(line *l)
{
    int i=0;
    char tmp_line[LINE_LEN];
    strcpy(tmp_line, l->processed_line);
    if (strstr(tmp_line, ".STRINGZ") != NULL)
    {
        //handle .STRINGZ specially
        char str[LINE_LEN];
        char *p = strchr(tmp_line, '\"');
        char *q = strrchr(tmp_line, '\"');
        p++; q--;
        int numbytes = (q-p+1)*sizeof(char);
        strncpy(str, p, numbytes);
        str[numbytes] = '\0';
        
        char *t = strtok(tmp_line, " ,");
        while (t != NULL)
        {
            strcpy(l->tokens[i], t);
            if (strcmp(l->tokens[i], ".STRINGZ") == 0)
            {
                break;
            }
            t = strtok(NULL, " ,");
            i++;
        }
        strcpy(l->tokens[i+1], str);
        l->tokens[i+2][0] = '\0';
    }
    else
    {
        char *p = strtok(tmp_line, " ,");
        while (p != NULL)
        {
            strcpy(l->tokens[i], p);
            i++;
            p = strtok(NULL, " ,");
        }
        l->tokens[i][0] = '\0';
    }
}

/*
 * 
 * name: islabel
 * The function determines if the passed string is a label or not by checking if its an OPCODE or an
 * assembler directive
 * 
 * @param: a string
 * @return: 1 if the string is a Label
 *          0 otherwise
 * 
 */
int islabel(char *token)
{
    int i;
    const char OPS[][TOK_LEN] = {"ADD", "AND", "BR", "JMP", "JSR", "JSRR", "LD", "LDI", "LDR", "LEA", "NOT",
                                 "RET", "RTI", "ST", "STI", "STR", "TRAP",
                                 "BR", "BRn", "BRz", "BRp", "BRnz", "BRzp", "BRnp", "BRnzp",
                                 ".FILL", ".BLKW", ".STRINGZ",
                                 "GETC", "OUT", "PUTS", "IN", "PUTSP", "HALT"};
    for (i = 0; i < 34; i++)
    {
        if (strcmp(token, OPS[i]) == 0)
        {
            return 0;
        }
    }
    return 1;
}

/*
 * 
 * name: scannumber
 * @param - a string containing a number in the following format:
 *          hex number - xNUM
 *          decimal number - #NUM,
 *          an integer to hold the scanned number
 * @output- puts the scanned number as an integer in parameter n
 * 
 */
void scannumber(char *token, int *n)
{
    char c;
    sscanf(token, "%c", &c);
    switch(c)
    {
        case '#':
            sscanf(token, "%*c%d", n);
            break;
        case 'x':
            sscanf(token, "%*c%x", n);
            break;
        default:
            *n = INT_MAX;
            break;
    };
}
