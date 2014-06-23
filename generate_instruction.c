/*
 * File: generate_instruction.c
 * Functions to convert instructions from assembly to the LC-3 ISA machine code in binary
 */

#include "lc3asm.h"

/*
 * 
 * name: generate_instruction
 * Parses the opcode in the assembly instruction and generates the binary machine code
 * Assembler directives (Pseudo-ops) are also covered except .BLKW and .STRINGZ
 * 
 */
int generate_instruction(char inst[], line *l, int opindex, symbol *symtable, int *LC)
{
    if (strcmp(l->tokens[opindex], "ADD") == 0)
    {
        strcpy(inst, "0001");
        char *DR, *SR1, *SR2, imm5[6];
        int imm5num;
        DR = regcode(l->tokens[opindex+1]);
        if (DR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for ADD\n");
            return 1;
        }
        strcat(inst, DR);
        SR1 = regcode(l->tokens[opindex+2]);
        if (SR1 == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for ADD\n");
            return 1;
        }
        strcat(inst, SR1);
        SR2 = regcode(l->tokens[opindex+3]);
        if (SR2 == NULL)
        {
            scannumber(l->tokens[opindex+3], &imm5num);
            if (imm5num == INT_MAX)
            {
                fprintf(stderr, "Error: Invalid operands for ADD\n");
                return 1;
            }
            else if (imm5num > 15 || imm5num < -16)
            {
                fprintf(stderr, "Error: Immediate operand for ADD out of range\n");
                return 1;
            }
            else
            {
                strcat(inst, "1");
                int_to_bin(imm5num, imm5, 5);
                strcat(inst, imm5);
            }
        }
        else
        {
            strcat(inst, "000");
            strcat(inst, SR2);
        }
        if (l->tokens[opindex+4][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for ADD\n");
            return 1;
        }
        
    }
    else if (strcmp(l->tokens[opindex], "AND") == 0)
    {
        strcpy(inst, "0101");
        char *DR, *SR1, *SR2, imm5[6];
        int imm5num;
        DR = regcode(l->tokens[opindex+1]);
        if (DR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for AND\n");
            return 1;
        }
        strcat(inst, DR);
        SR1 = regcode(l->tokens[opindex+2]);
        if (SR1 == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for AND\n");
            return 1;
        }
        strcat(inst, SR1);
        SR2 = regcode(l->tokens[opindex+3]);
        if (SR2 == NULL)
        {
            scannumber(l->tokens[opindex+3], &imm5num);
            if (imm5num == INT_MAX)
            {
                fprintf(stderr, "Error: Invalid operands for AND\n");
                return 1;
            }
            else if (imm5num > 15 || imm5num < -16)
            {
                fprintf(stderr, "Error: Immediate operand for AND out of range\n");
                return 1;
            }
            else
            {
                strcat(inst, "1");
                int_to_bin(imm5num, imm5, 5);
                strcat(inst, imm5);
            }
        }
        else
        {
            strcat(inst, "000");
            strcat(inst, SR2);
        }
        if (l->tokens[opindex+4][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for ADD\n");
            return 1;
        }
    }
    else if (strstr(l->tokens[opindex], "BR") != NULL)
    {
        strcpy(inst, "0000");
        if (strcmp(l->tokens[opindex], "BR") == 0)
        {
            strcat(inst, "111");
        }
        else if (strcmp(l->tokens[opindex], "BRn") == 0)
        {
            strcat(inst, "100");
        }
        else if (strcmp(l->tokens[opindex], "BRz") == 0)
        {
            strcat(inst, "010");
        }
        else if (strcmp(l->tokens[opindex], "BRp") == 0)
        {
            strcat(inst, "001");
        }
        else if (strcmp(l->tokens[opindex], "BRnz") == 0)
        {
            strcat(inst, "110");
        }
        else if (strcmp(l->tokens[opindex], "BRzp") == 0)
        {
            strcat(inst, "011");
        }
        else if (strcmp(l->tokens[opindex], "BRnp") == 0)
        {
            strcat(inst, "101");
        }
        else if (strcmp(l->tokens[opindex], "BRnzp") == 0)
        {
            strcat(inst, "111");
        }
        
        int offset;
        char PCoffset9[10];
        scannumber(l->tokens[opindex+1], &offset);
        if (offset != INT_MAX)
        {
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else if (islabel(l->tokens[opindex+1]))
        {
            int addr = search_table(symtable, l->tokens[opindex+1]);
            if (addr == -1)
            {
                fprintf(stderr, "Error: BR: Label not found in symbol table\n");
                return 1;
            }
            offset = addr - ((*LC)+1);
            if (offset > 255 || offset < -255)
            {
                fprintf(stderr, "Error: BR: offset out of range\n");
                return 1;
            }
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else
        {
            fprintf(stderr, "Error: Invalid operand for BR\n");
            return 1;
        }
        
        if (l->tokens[opindex+2][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operand for BR\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "JMP") == 0)
    {
        strcpy(inst, "1100");
        strcat(inst, "000");
        char *BaseR;
        BaseR = regcode(l->tokens[opindex+1]);
        if (BaseR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for JMP\n");
            return 1;
        }
        strcat(inst, BaseR);
        strcat(inst, "000000");
        if (l->tokens[opindex+2][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for JMP\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "JSR") == 0)
    {
        strcpy(inst, "0100");
        strcat(inst, "1");
        int offset;
        char PCoffset11[12];
        scannumber(l->tokens[opindex+1], &offset);
        if (offset != INT_MAX)
        {
            int_to_bin(offset, PCoffset11, 11);
            strcat(inst, PCoffset11);
        }
        else if (islabel(l->tokens[opindex+1]))
        {
            int addr = search_table(symtable, l->tokens[opindex+1]);
            if (addr == -1)
            {
                fprintf(stderr, "Error: JSR: Label not found in symbol table\n");
                return 1;
            }
            offset = addr - ((*LC)+1);
            if (offset > 255 || offset < -255)
            {
                fprintf(stderr, "Error: JSR: offset out of range\n");
                return 1;
            }
            int_to_bin(offset, PCoffset11, 11);
            strcat(inst, PCoffset11);
        }
        else
        {
            fprintf(stderr, "Error: Invalid operand for JSR\n");
            return 1;
        }
        
        if (l->tokens[opindex+2][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operand for JSR\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "JSRR") == 0)
    {
        strcpy(inst, "0100");
        strcat(inst, "000");
        char *BaseR;
        BaseR = regcode(l->tokens[opindex+1]);
        if (BaseR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for JSRR\n");
            return 1;
        }
        strcat(inst, BaseR);
        strcat(inst, "000000");
        if (l->tokens[opindex+2][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for JSRR\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "LD") == 0)
    {
        strcpy(inst, "0010");
        char *DR;
        DR = regcode(l->tokens[opindex+1]);
        if (DR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for LD\n");
            return 1;
        }
        strcat(inst, DR);
        
        int offset;
        char PCoffset9[10];
        scannumber(l->tokens[opindex+2], &offset);
        if (offset != INT_MAX)
        {
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else if (islabel(l->tokens[opindex+2]))
        {
            int addr = search_table(symtable, l->tokens[opindex+2]);
            if (addr == -1)
            {
                fprintf(stderr, "Error: LD: Label not found in symbol table\n");
                return 1;
            }
            offset = addr - ((*LC)+1);
            if (offset > 255 || offset < -255)
            {
                fprintf(stderr, "Error: LD: offset out of range\n");
                return 1;
            }
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else
        {
            fprintf(stderr, "Error: Invalid operands for LD\n");
            return 1;
        }
        
        if (l->tokens[opindex+3][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for LD\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "LDI") == 0)
    {
        strcpy(inst, "1010");
        char *DR;
        DR = regcode(l->tokens[opindex+1]);
        if (DR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for LD\n");
            return 1;
        }
        strcat(inst, DR);
        
        int offset;
        char PCoffset9[10];
        scannumber(l->tokens[opindex+2], &offset);
        if (offset != INT_MAX)
        {
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else if (islabel(l->tokens[opindex+2]))
        {
            int addr = search_table(symtable, l->tokens[opindex+2]);
            if (addr == -1)
            {
                fprintf(stderr, "Error: LDI: Label not found in symbol table\n");
                return 1;
            }
            offset = addr - ((*LC)+1);
            if (offset > 255 || offset < -255)
            {
                fprintf(stderr, "Error: LDI: offset out of range\n");
                return 1;
            }
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else
        {
            fprintf(stderr, "Error: Invalid operands for LDI\n");
            return 1;
        }
        
        if (l->tokens[opindex+3][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for LDI\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "LDR") == 0)
    {
        strcpy(inst, "0110");
        char *DR, *BaseR;
        DR = regcode(l->tokens[opindex+1]);
        if (DR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for STR\n");
            return 1;
        }
        strcat(inst, DR);
        
        BaseR = regcode(l->tokens[opindex+2]);
        if (BaseR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for STR\n");
            return 1;
        }
        strcat(inst, BaseR);
        
        int offset;
        char PCoffset6[7];
        scannumber(l->tokens[opindex+3], &offset);
        if (offset != INT_MAX)
        {
            int_to_bin(offset, PCoffset6, 6);
            strcat(inst, PCoffset6);
        }
        else if (islabel(l->tokens[opindex+2]))
        {
            int addr = search_table(symtable, l->tokens[opindex+2]);
            if (addr == -1)
            {
                fprintf(stderr, "Error: LDR: Label not found in symbol table\n");
                return 1;
            }
            offset = addr - ((*LC)+1);
            if (offset > 255 || offset < -255)
            {
                fprintf(stderr, "Error: LDR: offset out of range\n");
                return 1;
            }
            int_to_bin(offset, PCoffset6, 6);
            strcat(inst, PCoffset6);
        }
        else
        {
            fprintf(stderr, "Error: Invalid operands for LDR\n");
            return 1;
        }
        
        if (l->tokens[opindex+4][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for LDR\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "LEA") == 0)
    {
        strcpy(inst, "1110");
        char *DR;
        DR = regcode(l->tokens[opindex+1]);
        if (DR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for LEA\n");
            return 1;
        }
        strcat(inst, DR);
        
        int offset;
        char PCoffset9[10];
        scannumber(l->tokens[opindex+2], &offset);
        if (offset != INT_MAX)
        {
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else if (islabel(l->tokens[opindex+2]))
        {
            int addr = search_table(symtable, l->tokens[opindex+2]);
            if (addr == -1)
            {
                fprintf(stderr, "Error: LEA: Label not found in symbol table\n");
                return 1;
            }
            offset = addr - ((*LC)+1);
            if (offset > 255 || offset < -255)
            {
                fprintf(stderr, "Error: LEA: offset out of range\n");
                return 1;
            }
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else
        {
            fprintf(stderr, "Error: Invalid operands for LEA\n");
            return 1;
        }
        
        if (l->tokens[opindex+3][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for LEA\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "NOT") == 0)
    {
        strcpy(inst, "1001");
        char *DR, *SR;
        DR = regcode(l->tokens[opindex+1]);
        if (DR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for NOT\n");
            return 1;
        }
        strcat(inst, DR);
        SR = regcode(l->tokens[opindex+2]);
        if (SR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for NOT\n");
            return 1;
        }
        strcat(inst, SR);
        strcat(inst, "111111");
        if (l->tokens[opindex+3][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for NOT\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "RET") == 0)
    {
        strcpy(inst, "1100");
        strcat(inst, "000111000000");
        if (l->tokens[opindex+1][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid RET statement\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "RTI") == 0)
    {
        strcpy(inst, "1000");
        strcat(inst, "000000000000");
        if (l->tokens[opindex+1][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid RTI statement\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "ST") == 0)
    {
        strcpy(inst, "0011");
        char *SR;
        SR = regcode(l->tokens[opindex+1]);
        if (SR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for ST\n");
            return 1;
        }
        strcat(inst, SR);
        
        int offset;
        char PCoffset9[10];
        scannumber(l->tokens[opindex+2], &offset);
        if (offset != INT_MAX)
        {
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else if (islabel(l->tokens[opindex+2]))
        {
            int addr = search_table(symtable, l->tokens[opindex+2]);
            if (addr == -1)
            {
                fprintf(stderr, "Error: ST: Label not found in symbol table\n");
                return 1;
            }
            offset = addr - ((*LC)+1);
            if (offset > 255 || offset < -255)
            {
                fprintf(stderr, "Error: ST: offset out of range\n");
                return 1;
            }
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else
        {
            fprintf(stderr, "Error: Invalid operands for ST\n");
            return 1;
        }
        
        if (l->tokens[opindex+3][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for ST\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "STI") == 0)
    {
        strcpy(inst, "1011");
        char *SR;
        SR = regcode(l->tokens[opindex+1]);
        if (SR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for STI\n");
            return 1;
        }
        strcat(inst, SR);
        
        int offset;
        char PCoffset9[10];
        scannumber(l->tokens[opindex+2], &offset);
        if (offset != INT_MAX)
        {
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else if (islabel(l->tokens[opindex+2]))
        {
            int addr = search_table(symtable, l->tokens[opindex+2]);
            if (addr == -1)
            {
                fprintf(stderr, "Error: STI: Label not found in symbol table\n");
                return 1;
            }
            offset = addr - ((*LC)+1);
            if (offset > 255 || offset < -255)
            {
                fprintf(stderr, "Error: STI: offset out of range\n");
                return 1;
            }
            int_to_bin(offset, PCoffset9, 9);
            strcat(inst, PCoffset9);
        }
        else
        {
            fprintf(stderr, "Error: Invalid operands for STI\n");
            return 1;
        }
        
        if (l->tokens[opindex+3][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for STI\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "STR") == 0)
    {
        strcpy(inst, "0111");
        char *SR, *BaseR;
        SR = regcode(l->tokens[opindex+1]);
        if (SR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for STR\n");
            return 1;
        }
        strcat(inst, SR);
        
        BaseR = regcode(l->tokens[opindex+2]);
        if (BaseR == NULL)
        {
            fprintf(stderr, "Error: Invalid operands for STR\n");
            return 1;
        }
        strcat(inst, BaseR);
        
        int offset;
        char PCoffset6[7];
        scannumber(l->tokens[opindex+3], &offset);
        if (offset != INT_MAX)
        {
            int_to_bin(offset, PCoffset6, 6);
            strcat(inst, PCoffset6);
        }
        else if (islabel(l->tokens[opindex+2]))
        {
            int addr = search_table(symtable, l->tokens[opindex+2]);
            if (addr == -1)
            {
                fprintf(stderr, "Error: STR: Label not found in symbol table\n");
                return 1;
            }
            offset = addr - ((*LC)+1);
            if (offset > 255 || offset < -255)
            {
                fprintf(stderr, "Error: STR: offset out of range\n");
                return 1;
            }
            int_to_bin(offset, PCoffset6, 6);
            strcat(inst, PCoffset6);
        }
        else
        {
            fprintf(stderr, "Error: Invalid operands for STR\n");
            return 1;
        }
        
        if (l->tokens[opindex+4][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operands for STR\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "TRAP") == 0)
    {
        strcpy(inst, "1111");
        strcat(inst, "0000");
        int trapvector;
        char trapvect8[9];
        scannumber(l->tokens[opindex+1], &trapvector);
        if (trapvector == INT_MAX)
        {
            fprintf(stderr, "Error: Invalid operand for TRAP\n");
            return 1;
        }
        if (l->tokens[opindex+2][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid operand for TRAP\n");
            return 1;
        }
        int_to_bin(trapvector, trapvect8, 8);
        strcat(inst, trapvect8);
    }
    else if (strcmp(l->tokens[opindex], ".FILL") == 0)
    {
        int value;
        char fillvalue[17];
        scannumber(l->tokens[opindex+1], &value);
        if (value != INT_MAX)
        {
            int_to_bin(value, fillvalue, 16);
            strcpy(inst, fillvalue);
        }
        else if (islabel(l->tokens[opindex+1]))
        {
            int addr = search_table(symtable, l->tokens[opindex+1]);
            if (addr == -1)
            {
                fprintf(stderr, "Error: .FILL: Label not found in symbol table\n");
                return 1;
            }
            value = addr;
            int_to_bin(value, fillvalue, 16);
            strcpy(inst, fillvalue);
        }
        else
        {
            fprintf(stderr, "Error: Invalid argument to .FILL\n");
            return 1;
        }
        
        if (l->tokens[opindex+2][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid argument to .FILL\n");
            return 1;
        }
    }
    else if (strcmp(l->tokens[opindex], "GETC") == 0)
    {
        strcpy(inst, "1111");
        strcat(inst, "0000");
        int trapvector;
        char trapvect8[9];
        scannumber("x20", &trapvector);
        if (l->tokens[opindex+1][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid GETC statement\n");
            return 1;
        }
        int_to_bin(trapvector, trapvect8, 8);
        strcat(inst, trapvect8);
    }
    else if (strcmp(l->tokens[opindex], "OUT") == 0)
    {
        strcpy(inst, "1111");
        strcat(inst, "0000");
        int trapvector;
        char trapvect8[9];
        scannumber("x21", &trapvector);
        if (l->tokens[opindex+1][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid OUT statement\n");
            return 1;
        }
        int_to_bin(trapvector, trapvect8, 8);
        strcat(inst, trapvect8);
    }
    else if (strcmp(l->tokens[opindex], "PUTS") == 0)
    {
        strcpy(inst, "1111");
        strcat(inst, "0000");
        int trapvector;
        char trapvect8[9];
        scannumber("x22", &trapvector);
        if (l->tokens[opindex+1][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid PUTS statement\n");
            return 1;
        }
        int_to_bin(trapvector, trapvect8, 8);
        strcat(inst, trapvect8);
    }
    else if (strcmp(l->tokens[opindex], "IN") == 0)
    {
        strcpy(inst, "1111");
        strcat(inst, "0000");
        int trapvector;
        char trapvect8[9];
        scannumber("x23", &trapvector);
        if (l->tokens[opindex+1][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid IN statement\n");
            return 1;
        }
        int_to_bin(trapvector, trapvect8, 8);
        strcat(inst, trapvect8);
    }
    else if (strcmp(l->tokens[opindex], "PUTSP") == 0)
    {
        strcpy(inst, "1111");
        strcat(inst, "0000");
        int trapvector;
        char trapvect8[9];
        scannumber("x24", &trapvector);
        if (l->tokens[opindex+1][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid PUTSP statement\n");
            return 1;
        }
        int_to_bin(trapvector, trapvect8, 8);
        strcat(inst, trapvect8);
    }
    else if (strcmp(l->tokens[opindex], "HALT") == 0)
    {
        strcpy(inst, "1111");
        strcat(inst, "0000");
        int trapvector;
        char trapvect8[9];
        scannumber("x25", &trapvector);
        if (l->tokens[opindex+1][0] != '\0')
        {
            fprintf(stderr, "Error: Invalid HALT statement\n");
            return 1;
        }
        int_to_bin(trapvector, trapvect8, 8);
        strcat(inst, trapvect8);
    }
    else
    {
        fprintf(stderr, "Error: Illegal instruction\n");
        return 1;
    }
    return 0;
}

/*
 * 
 * name: handle_BLKW
 * function to handle generation of machine code for the .BLKW directive. A separate function was needed because
 * one line of assembly produces multiple lines of machine code. Prints to the output file and increments Location
 * Counter (LC) appropriately.
 * 
 */
int handle_BLKW(files *f, line *l, int opindex, int *LC)
{
    if (l->tokens[opindex+1][0] == '\0')
    {
        fprintf(stderr, "Illegal argument to .BLKW directive\n");
        return 1;
    }
    int block;
    scannumber(l->tokens[opindex+1], &block);
    if (block == INT_MAX)
    {
        fprintf(stderr, "Error: Invalid argument to .BLKW directive\n");
        return 1;
    }
    if (l->tokens[opindex+2][0] != '\0')
    {
        fprintf(stderr, "Error: Invalid argument to .BLKW directive\n");
        return 1;
    }
    
    int i;
    for (i = 0; i < block; i++)
    {
        fprintf(f->outbin, "0000000000000000\n");
        fprintf(f->outhex, "x0000\n");
        *LC = *LC + 1;
    }
    
    return 0;
}

/*
 * 
 * name: handle_STRINGZ
 * function to handle generation of machine code for the .STRINGZ directive. A separate function was needed because
 * one line of assembly produces multiple lines of machine code. Prints to the output file and increments Location
 * Counter (LC) appropriately.
 * 
 */
int handle_STRINGZ(files *f, line *l, int opindex, int *LC)
{
    if (l->tokens[opindex+1][0] == '\0')
    {
        fprintf(stderr, "Illegal argument to .STRINGZ directive\n");
        return 1;
    }
    
    if (l->tokens[opindex+2][0] != '\0')
    {
        fprintf(stderr, "Error: Invalid argument to .STRINGZ directive\n");
        return 1;
    }
    
    int i;
    int char_ascii;
    char character_bin[17], character_hex[17];
    
    for (i = 0; i < strlen(l->tokens[opindex+1]); i++)
    {
        char_ascii = (int)(l->tokens[opindex+1][i]);
        int_to_bin(char_ascii, character_bin, 16);
        fprintf(f->outbin, "%s\n", character_bin);
        instruction_bintohex(character_bin, character_hex);
        fprintf(f->outhex, "%s\n", character_hex);
        
        *LC = *LC + 1;
    }
    
    //print sentinel - x0000
    strcpy(character_bin, "0000000000000000");
    fprintf(f->outbin, "%s\n", character_bin);
    instruction_bintohex(character_bin, character_hex);
    fprintf(f->outhex, "%s\n", character_hex);
    *LC = *LC + 1;
    
    return 0;
}
