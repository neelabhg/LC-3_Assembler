================================
LC-3 Assembler
================================
README
Author: Neelabh Gupta (nsgupta2@illinois.edu)

About
--------------------------------
This is an assembler for the LC-3 ISA. It was developed as an honors project for the ECE 190 (Introduction to Computing Systems) Spring 2012 class (https://wiki.engr.illinois.edu/display/ece190sp12/Home) at the University of Illinois at Urbana-Champaign. It is completely written in the C language, uses only the standard C libraries, and conforms to the C99 standard.
Developed by Neelabh Gupta (nsgupta2@illinois.edu).

NOTE: This project should only be used for educational purposes. For practical use, use the assembler provided by the official LC-3 tools (see below).


About the LC-3
--------------------------------
LC-3, or Little Computer 3, is a simple computer built by Yale N. Patt and Sanjay J. Patel for use in teaching fundamentals of programming and computer architecture to students. Their specification of the instruction set, the overall architecture of the LC-3, and a hardware implementation can be found in the second edition of their textbook, "Introduction to Computing Systems: From Bits and Gates to C and Beyond".

- The main textbook website containing links to all resources can be found here: http://highered.mcgraw-hill.com/sites/0072467509/index.html
- The LC-3 ISA (instructions, opcodes and formats) can be found in Appendix A, available for download here: http://highered.mcgraw-hill.com/sites/dl/free/0072467509/104653/PattPatelAppA.pdf
- The LC-3 tools, containing a C compiler, assembler (lc3as), bin to obj convertor (lc3convert), the simulator and other tools can be found here: http://highered.mcgraw-hill.com/sites/0072467509/student_view0/lc-3_simulator.html
- The Wikipedia page for LC-3 is also useful: http://en.wikipedia.org/wiki/LC-3


Functionality:
--------------------------------
Takes an assembly language file <filename.asm> and assembles it into LC-3 ISA machine code. The machine code is in "filename.bin" and its corresponding hexadecimal code is in "filename_hex.bin". Assembly stops and an appropriate error message is printed when an error in the assembly code is encountered.


Initial Setup:
--------------------------------
The source files can be compiled using the "compile" file in the root of the source folder. Its just a line containing the gcc command with appropriate arguments.

First make the compile script executable:
$> chmod 700 compile

Compile the source code:
$> ./compile

The output file is named "nglc3asm". Place this executable somewhere on your PATH, so that you don't need to type its path every time you run it.


Usage:
--------------------------------
To run the program,
$> ./nglc3asm <path/to/filename.asm>

In case of an error during assembly, an error message will be printed on screen indicating the error occurred, and the assembly process stops.

If the assembly was successful, a success message will be printed on the screen.
The output files will be:
path/to/filename.bin        - the main binary code produced
path/to/filename_hex.bin    - the machine code in hexadecimal
path/to/filename.sym        - the file containing the symbol table

All these files are human-readable - open them in any text-editor. To generate an object file, use a binary to .obj convertor. The standard LC-3 tools include a tool named "lc3convert".

On a system where the LC-3 tools are installed, type the following command:
$> lc3convert path/to/filename.bin

This will produce a .obj file, which can then be loaded onto the LC-3 simulator (provided by LC-3 tools).


Organization of source code:
--------------------------------
LC-3_Assembler/
   - README.txt                      - This readme
   - compile                         - script to compile all source files
   - lc3asm.h                        - header file acting as a "glue"
   - main.c                          - contains the main function
   - assemble.c                      - source file
   - generate_instruction.c          - source file
   - instruction.c                   - source file
   - parse.c                         - source file
   - symbol.c                        - source file
   - test_cases/                     - Directory containing some sample assembly source code, and the expected outputs of the assembler


Assembler Specific Notes
--------------------------------
.STRINGZ assembler directive can have string arguments having escape sequences ("\n", etc). The parsed escape sequences are:
'\a', '\b', '\f', '\n', '\r', '\t', '\v', '\\', '\?', '\'', '\"'

Numbers in instructions can be given in the following format:
If the operand is a decimal integer, it should be preceded with a "#". For example, ADD R0, R0, #7
If the operand is a hexadecimal integer, it should be preceded with an "x". For example, .ORIG x3000


Future Improvements
--------------------------------
- Show line numbers for errors.
- This assembler stops when it encounters the first error. The assembler should keep moving and find all errors.
- Create a ".obj" file output which can be loaded directly into the simulator without the need for "lc3convert".
