/*
 * Main file
 */

#include "lc3asm.h"

int main(int argc, char *argv[])
{
    char *p;
    files f;
    
    if (argc == 2)
    {
        //open input file and output files with appropriate names/extensions in the source file directory
        strcpy(f.infilename, argv[1]);
        p = strstr(f.infilename, ".asm");
        if (p == NULL)
        {
            printf("Please input a file in assembly format\n");
            return 1;
        }
        else
        {
            strcpy(f.outhexfilename, f.infilename);
            p = strstr(f.outhexfilename, ".asm");
            strcpy(p, "_hex.bin");
            
            strcpy(f.outbinfilename, f.infilename);
            p = strstr(f.outbinfilename, ".asm");
            strcpy(p, ".bin");
            
            strcpy(f.symfilename, f.infilename);
            p = strstr(f.symfilename, ".asm");
            strcpy(p, ".sym");
            
            f.in = fopen(f.infilename, "r");
            if (f.in == NULL)
            {
                printf("Error while opening input file \"%s\"\n", f.infilename);
                return 1;
            }
            
            f.outhex = fopen(f.outhexfilename, "w");
            f.outbin = fopen(f.outbinfilename, "w");
            f.symfile = fopen(f.symfilename, "w");
            
            if (f.outhex == NULL || f.outbin == NULL || f.symfile == NULL)
            {
                printf("Error while opening output files\n");
                printf("You possibly don't have write permissions for the source directory.\n");
                return 1;
            }
        }
    }
    else
    {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }
    
    //call the main assembler function
    assemble(&f);
    
    fclose(f.in);
    fclose(f.outhex);
    fclose(f.outbin);
    fclose(f.symfile);
    return 0;
}
