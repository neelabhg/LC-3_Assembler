/*
 * File: assemble.c
 * Contains the entry point for the assembly process and functions which conduct the first and second passes
 * of the assembly process.
 */

#include "lc3asm.h"

/*
 * 
 * name: assemble
 * Invokes firstpass() and secondpass(). This is the function invoked by main()
 * 
 */
void assemble(files *f)
{
    int success;
    printf("Assembling \"%s\"\n\n", f->infilename);
    printf("First Pass\n");
    printf("----------\n");
    symbol *symtable = NULL;
    firstpass(f, &symtable, &success);
    if (!success)
    {
        printf("First Pass failed\n");
        return;
    }
    printf("First Pass successful\n");
    write_symfile(symtable, f);
    printf("Symbol table written to file \"%s\"\n", f->symfilename);
    printf("\nSecond Pass\n");
    printf("-----------\n");
    secondpass(f, &symtable, &success);
    if (!success)
    {
        printf("Second Pass failed\n");
        return;
    }
    printf("Second Pass successful\n");
    printf("Binary file written to file \"%s\"\n", f->outbinfilename);
    printf("Hexadecimal file written to file \"%s\"\n", f->outhexfilename);
    destroy_symtable(symtable);
}

/*
 * 
 * name: firstpass
 * The first pass of the assembly process. Searches for the ".ORIG" directive, and starts adding labels to the symbol
 * table. Then searches for a ".END" directive.
 * 
 */
void firstpass(files *f, symbol **symtable, int *success)
{
    line l;
    int LC;
    int foundORIG = 0, foundEND = 0;
    char op[TOK_LEN];
    int opindex;
    
    *success = 0;
    
    while (!feof(f->in))
    {
        readline(&l, f);
        parsetokens(&l);
        if (strcmp(l.tokens[0], ".ORIG") == 0)
        {
            foundORIG = 1;
            if (l.tokens[1][0] != '\0')
            {
                scannumber(l.tokens[1], &LC);
                break;
            }
            else
            {
                fprintf(stderr, "Invalid .ORIG statement\n");
                return;
            }
        }
    }
    if (!foundORIG)
    {
        fprintf(stderr, ".ORIG pseudo-op not found.\n");
        return;
    }
    
    //.ORIG is found and position indicator is on line below .ORIG statement
    while (!feof(f->in))
    {
        readline(&l, f);
        parsetokens(&l);
        
        if (strcmp(l.tokens[0], ".END") == 0)
        {
            foundEND = 1;
            break;
        }
        
        if (l.tokens[0][0] != '\0')
        {
            if (islabel(l.tokens[0]))
            {
                if (search_table(*symtable, l.tokens[0]) != -1)
                {
                    fprintf(stderr, "Label %s has already appeared in file\n", l.tokens[0]);
                    return;
                }
                append_symtable(symtable, l.tokens[0], LC);
                if (l.tokens[1][0] != '\0')
                {
                    opindex = 1;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                opindex = 0;
            }
            
            strcpy(op, l.tokens[opindex]);
            
            if (strcmp(op, ".BLKW") == 0)
            {
                if (l.tokens[opindex+1][0] == '\0')
                {
                    fprintf(stderr, "Illegal argument to .BLKW directive\n");
                    return;
                }
                int block;
                scannumber(l.tokens[opindex+1], &block);
                LC += block;
            }
            else if (strcmp(op, ".STRINGZ") == 0)
            {
                if (l.tokens[opindex+1][0] == '\0')
                {
                    fprintf(stderr, "Illegal argument to .STRINGZ directive\n");
                    return;
                }
                
                LC += strlen(l.tokens[opindex+1]) + 1;
            }
            else
            {
                LC++;
            }
        }
    }
    
    if (!foundEND)
    {
        fprintf(stderr, ".END pseudo-op not found.\n");
        return;
    }
    
    *success = 1;
}

/*
 * 
 * name: secondpass
 * The second pass of the assembly process. Searches for ".ORIG", and starts generating instructions from there.
 * Searches the symbol table for addresses of symbols. Writes the LC-3 machine code in binary and hexadecimal to the
 * respective files.
 * 
 */
void secondpass(files *f, symbol **symtable, int *success)
{
    line l;
    int LC;
    int opindex;
    char op[TOK_LEN];
    *success = 0;
    
    rewind(f->in);
    
    while (!feof(f->in))
    {
        readline(&l, f);
        parsetokens(&l);
        if (strcmp(l.tokens[0], ".ORIG") == 0)
        {
            if (l.tokens[1][0] != '\0')
            {
                scannumber(l.tokens[1], &LC);
                char origlocationbin[17], origlocationhex[17];
                int_to_bin(LC, origlocationbin, 16);
                fprintf(f->outbin, "%s\n", origlocationbin);
                instruction_bintohex(origlocationbin, origlocationhex);
                fprintf(f->outhex, "%s\n", origlocationhex);
                break;
            }
        }
    }
    //position indicator is on line below .ORIG statement
    while (!feof(f->in))
    {
        readline(&l, f);
        parsetokens(&l);
        
        //finish reading if .END is encountered
        if (strcmp(l.tokens[0], ".END") == 0)
        {
            break;
        }
        
        //continue only if line is not empty
        if (l.tokens[0][0] != '\0')
        {
            /*
            fprintf(f->outbin, "%d ", LC);
            for (int j=0; l.tokens[j][0] != '\0'; j++)
            {
                fprintf(f->outbin, "[%s]", l.tokens[j]);
            }
            fprintf(f->outbin, "\n");
            */
            
            if (islabel(l.tokens[0]))
            {
                if (l.tokens[1][0] != '\0')
                {
                    opindex = 1;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                opindex = 0;
            }
            
            int error;
            strcpy(op, l.tokens[opindex]);
            if (strcmp(op, ".BLKW") == 0)
            {
                error = handle_BLKW(f, &l, opindex, &LC);
                if (error)
                {
                    printf("Generation of Instruction failed\n");
                    *success = 0;
                    return;
                }
            }
            else if (strcmp(op, ".STRINGZ") == 0)
            {
                error = handle_STRINGZ(f, &l, opindex, &LC);
                if (error)
                {
                    printf("Generation of Instruction failed\n");
                    *success = 0;
                    return;
                }
            }
            else
            {
                char inst_bin[17], inst_hex[5];
                error = generate_instruction(inst_bin, &l, opindex, *symtable, &LC);
                if (error)
                {
                    printf("Generation of Instruction failed\n");
                    *success = 0;
                    return;
                }
                fprintf(f->outbin, "%s\n", inst_bin);
                instruction_bintohex(inst_bin, inst_hex);
                fprintf(f->outhex, "%s\n", inst_hex);
                LC++;
            }
        }
    }
    
    *success = 1;
}
