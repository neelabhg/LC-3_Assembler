================================
LC-3 Assembler
================================
README

Author - Neelabh Gupta (nsgupta2@illinois.edu)

Functionality:
--------------------------------
Takes an assembly language file <filename.asm> and assembles it into LC-3 ISA machine code
The machine code is in "filename.bin" and its corresponding hexadecimal code is in "filename_hex.bin".
Assembly stops and an appropriate error message is printed when an error in the assembly code is encountered.

Initial Setup:
--------------------------------
The source files can be compiled using the "compile" file in the root of the source folder. Its just a line containing
the gcc command with appropriate arguments.

First make the compile script executable
$> chmod 700 compile

Compile the source code
$> ./compile

The output file is named "nglc3asm". "ng" stands for "Neelabh Gupta" (my name).

Usage:
--------------------------------
To run the program, 
$> ./nglc3asm <path/to/filename.asm>

In case of an error during assembly, an error message will be printed on screen indicating the error occurred, and
the assembly process stops.

If the assembly was successful, a success message will be printed on the screen.
The output files will be:
path/to/filename.bin        - the main binary code produced
path/to/filename_hex.bin    - the machine code in hexadecimal
path/to/filename.sym        - the file containing the symbol table

All these files are human-readable - open them in any text-editor
To generate an object file, use a binary to .obj convertor
The standard LC-3 tools include a tool named "lc3convert"
On a system where the LC-3 tools are installed, do

&> lc3convert path/to/filename.bin
This will produce a .obj file, which can then be loaded onto the LC-3 simulator (again provided by LC-3 tools).


Organization of source code:
--------------------------------
nsgupta2_lc3assembler/
   - README.txt                      - This readme
   - compile                         - script to compile all source files
   - lc3asm.h                        - header file acting as a "glue"
   - main.c                          - contains the main function
   - assemble.c
   - generate_instruction.c
   - instruction.c
   - parse.c
   - symbol.c


Assembler Specific Notes
--------------------------------
.STRINGZ assembler directive can have string arguments having escape sequences ("\n", etc).
The parsed escape sequences are:
'\a', '\b', '\f', '\n', '\r', '\t', '\v', '\\', '\?', '\'', '\"'

Numbers in instructions can be given in the following format:
If the operand is a decimal integer, it should be preceded with a "#". For example, ADD R0, R0, #7
If the operand is a hexadecimal integer, it should be preceded with an "x". For example, .ORIG x3000

Links
--------------------------------
The LC-3 tools, containing the simulator and other tools can be found here:
http://highered.mcgraw-hill.com/sites/0072467509/student_view0/lc-3_simulator.html

Info about the LC-3 can be found here: http://en.wikipedia.org/wiki/LC-3


About
--------------------------------
Developed by Neelabh Gupta (nsgupta2@illinois.edu).
This assembler was created as an honors project for the ECE 190 Spring 2012 class at the 
University of Illinois Urbana Champaign.

It is completely coded in the C language and uses the standard C libraries. It was compiled using "gcc" with the
C99 standard.
