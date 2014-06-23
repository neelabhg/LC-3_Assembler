/*
 * File: instruction.c
 * Contains functions to help in the instruction generation process
 */
 
#include "lc3asm.h"

/*
 * 
 * name: regcode
 * Tries to match "token" with a register and returns its corresponding code. NULL otherwise.
 * 
 */
char *regcode(char *token)
{
    if (strcmp(token, "R0") == 0)
    {
        return "000";
    }
    else if (strcmp(token, "R1") == 0)
    {
        return "001";
    }
    else if (strcmp(token, "R2") == 0)
    {
        return "010";
    }
    else if (strcmp(token, "R3") == 0)
    {
        return "011";
    }
    else if (strcmp(token, "R4") == 0)
    {
        return "100";
    }
    else if (strcmp(token, "R5") == 0)
    {
        return "101";
    }
    else if (strcmp(token, "R6") == 0)
    {
        return "110";
    }
    else if (strcmp(token, "R7") == 0)
    {
        return "111";
    }
    else
    {
        return NULL;
    }
}

/* Create a string of binary digits based on the input value.
   Input:
       val:  value to convert.
       buff: buffer to write to must be >= sz+1 chars.
       sz:   size of buffer.
   Returns address of string or NULL if not enough space provided.
*/
char *binrep (unsigned int val, char *buff, int sz) {
    /* Credit to User "paxdiablo" at stackoverflow
     * Permanant link to his code - http://stackoverflow.com/a/700018
     */
    
    char *pbuff = buff;

    /* Must be able to store one character at least. */
    if (sz < 1) return NULL;

    /* Special case for zero to ensure some output. */
    if (val == 0) {
        *pbuff++ = '0';
        *pbuff = '\0';
        return buff;
    }

    /* Work from the end of the buffer back. */
    pbuff += sz;
    *pbuff-- = '\0';

    /* For each bit (going backwards) store character. */
    while (val != 0) {
        if (sz-- == 0) return NULL;
        *pbuff-- = ((val & 1) == 1) ? '1' : '0';

        /* Get next bit. */
        val >>= 1;
    }
    return pbuff+1;
}

/*
 * 
 * name: int_to_bin
 * Takes an integer "val" and writes its binary string representation to "buff". The number of bits is "sz". The
 * assumes that "buff" has size >= sz+1
 * 
 * Code adapted from a solution posted on stackoverflow.
 * Credit to User "paxdiablo" at stackoverflow
 * Permanant link to his code - http://stackoverflow.com/a/700018
 * 
 * 
 */
char *int_to_bin(int val, char *buff, int sz)
{
    //assumption - size of buffer is sz+1
    
    char *pbuff = buff;

    /* Work from the end of the buffer back. */
    pbuff += sz;
    *pbuff = '\0';
    pbuff--;

    /* For each bit (going backwards) store character. */
    int i;
    for (i = 0; i < sz; i++)
    {
        *pbuff = ((val & 1) == 1) ? '1' : '0';
        pbuff--;
        /* Get next bit. */
        val = (val >> 1);
    }
    return pbuff+1;
}

/*
 * 
 * name: instruction_bintohex
 * Takes a string containing 16 bits and converts into its hexadecimal equivalent string representation.
 * 
 * Code adapted from a solution posted on stackoverflow.
 * Credit to user "CyberDem0n" at stackoverflow
 * Permanant link to his code - http://stackoverflow.com/a/5307692 
 * 
 * 
 */
void instruction_bintohex(char *inst_bin, char *inst_hex)
{
    char *a = inst_bin;
    int num = 0;
    int b;
    
    do {
        b = (*a == '1') ? 1 : 0;
        num = (num<<1) | b;
        a++;
    } while (*a);
    
    sprintf(inst_hex, "x%04X", num);
}
