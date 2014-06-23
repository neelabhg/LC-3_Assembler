; command line interface for the reverse polish calculator
;
; students MAY NOT MODIFY THIS FILE.
; students whose submissions depend on a modified version of this file
; are liable to receive a large penalty on their functionality score,
; and may receive a zero.

        .ORIG x3000

RESET   BRnzp PROMPT   ; jumped to by the student code
        BRnzp PRNTNUML ; jumped to by the student code

HANDLENUMREF .FILL x4000
HANDLEOPREF  .FILL x4001
HANDLEENDREF .FILL x4002

; first get the string from the user and put on a buffer.
; then parse char by char, calling HANDLENUM, HANDLEOP
; and HANDLENL appropriately.
PROMPT  LEA R0,PROMPTS
        PUTS
        LD R1,BUFFLEN 
        LEA R2,STRBUFF
GETS1   ADD R1,R1,#-1
        BRz GETS2 ; don't let buffer overrun
        GETC
        ADD R3,R0,#-8 ; check for backspace
        BRz GETS1C
        LD R3,DEL
        ADD R3,R0,R3 ; check for DEL
        BRnp GETS1B ; branch away if not DEL
GETS1C  ADD R1,R1,#1
        LD R3,BUFFLEN ; check to make sure we don't backspace past the beginning
        NOT R3,R3
        ADD R3,R3,#1
        ADD R3,R3,R1
        BRn GETS1A ; branch if backspace was legal
        LD R1,BUFFLEN ; else reset so we're still at starting point
        LEA R2,STRBUFF
        BRnzp GETS1
GETS1A  ADD R2,R2,#-1
        ADD R1,R1,#1
        AND R0,R0,#0
        ADD R0,R0,#8
        OUT ; echo the backspace to the console
        LD R0,SPACE
        OUT ; overwrite that spot with a space
        AND R0,R0,#0
        ADD R0,R0,#8
        OUT ; one more backspaces
        BRnzp GETS1

GETS1B  OUT ; echo character to console
        STR R0,R2,#0
        ADD R2,R2,#1
        ADD R3,R0,#-10 ; check if newline '\n'
        BRz GETS3
        ADD R3,R0,#-13 ; check if carriage return '\r'
        BRz GETS3        
        BRnzp GETS1
GETS2   AND R0,R0,#0 ; put a newline at the end if buffer limit is reached
        ADD R0,R0,#10
        OUT
        STR R0,R2,#0
        ADD R2,R2,#1
GETS3   AND R0,R0,#0
        STR R0,R2,#0 ; ensure string is null-terminated

        AND R0,R0,#0
        ST R0,PASSNO
PSTART  LEA R0,STRBUFF
        AND R5,R5,#0
        ADD R5,R5,R0 ; put string pointer in R5
        AND R0,R0,#0
        ST R0,PNUMSUM ; reset "numsum" ie sum for numeral parsing
        ADD R0,R0,x1
        ST R0,PSTATE ; reset parser state (default state = x01)

PNEXT   LDR R0,R5,#0    ; grab next character from strbuff for parsing, put in R0
        ADD R5,R5,#1    ; increment string pointer R5
        LEA R1,PCHARS   ; get list of valid characters (put pointer in R1)
PLOOP0  LDR R2,R1,#0    ; loop through list of characters, trying to match input char
        BRz PRSERR      ; throw parse error if char is not in the valid list
        ADD R1,R1,#1
        NOT R2,R2
        ADD R2,R2,#1
        ADD R2,R2,R0
        BRnp PLOOP0     ; if input char doesn't match this char, branch back up to try next

        ADD R1,R1,#-1   ; undo last "extra" increment so R1 points to the matching char
        LEA R4,PCHARS   ; subtract base pointer
        NOT R4,R4
        ADD R4,R4,#1
        ADD R1,R4,R1    ; so R1 now holds the input char's index in our list of valid chars
    
        AND R4,R4,#0    ; now we need to identify the character's type
        ADD R4,R4,#-1   ; R4 will store the type; start at -1 so we can inc to 0 first time thru the loop
        LEA R2,PCOUNTS  ; R2 now holds pointer to the number of characters in type 0
PLOOP1  ADD R4,R4,#1
        LDR R3,R2,#0    ; R3 is now the number of elements in the type
        ADD R2,R2,#1
        NOT R3,R3
        ADD R3,R3,#1
        ADD R1,R3,R1
        BRzp PLOOP1     ; if R1 went negative, it means this is the correct Type to which the char belongs
        NOT R3,R3
        ADD R3,R3,#1    ; add the count back onto R1
        ADD R1,R3,R1    ; so that now R4 contains the type, and R1 the subindex

PARSE0  LD R2,PSTATE  ; other_operator|minus|numeral|ready. valid states: 0001, 0010, 0110, 0100, 1000
        LD R3,PNUMSUM ; load up the state from the last char we parsed

PARSE1  AND R4,R4,R4 ; if char is a numeral
        BRp PARSE2 
        AND R0,R2,x7 ; check whether we are in numeral-processing state, ready state, or post-minus
        BRz PRSERR ; (if not, we have a parse error)
        AND R2,R2,x4 ; if so, we are now in numeral-processing state, keeping minus bit if applicable
        ADD R2,R2,x2 
        AND R6,R6,#0 ; need to shift R3 to the left (decimal)
        AND R0,R0,#0 
        ADD R0,R0,#10
PARSE1A ADD R0,R0,#-1
        BRn PARSE1B
        ADD R6,R6,R3
        BRnzp PARSE1A
PARSE1B AND R0,R2,x4 ; check if num we are parsing is supposed to be negative
        BRz PARSE1C
        NOT R1,R1
        ADD R1,R1,#1
PARSE1C ADD R3,R1,R6 ; add this digit to the sum total
        BRnzp PARSE9

PARSE2  ADD R4,R4,#-1 ;if char is minus
        BRp PARSE3
        AND R0,R2,x1
        BRz PRSERR ; minus is only allowed in the ready state, else parse error
        AND R2,R2,#0
        ADD R2,R2,x4 ; set state to post-minus. now we need to wait to see whether it's a negative or operator
        BRnzp PARSE9

PARSE3  ADD R4,R4,#-1 ;if other kind of operator
        BRp PARSE4
        AND R0,R2,x1
        BRz PRSERR ; operator only allowed in ready state.
        AND R2,R2,#0
        ADD R2,R2,#8 ; set state to post-operator
        AND R1,R1,R1
        BRz PARSE3A
        ADD R1,R1,#1 ; add 1 to index of operator_id if not 0 (we insert - as index 1)
PARSE3A LD R0,PASSNO ; dont call handlers in pass 0
        AND R0,R0,R0
        BRz PARSE9
        JSR CALLSV
        LDI R0,HANDLEOPREF
        JSRR R0 ; handle operator right away in this case. for minus, we wait for following whitespace
        JSR CALLLD
        BRnzp PARSE9    

PARSE4  ; if the char is a space or a newline.  these should always be legal and never cause a parse error.
        AND R0,R2,x6 ; interesting things only happen if we are in numeral or post-minus state
        BRz PARSE4C  ; branch if state is neither in numeral-processing or post-minus
        AND R0,R2,x2 ; check whether it was a numeral
        BRz PARSE4A  ; branch if not
        ADD R1,R3,#0 ; put numeral value in R1
        AND R3,R3,#0 ; zero out the numsum
        LD R0,PASSNO ; dont call handlers in pass 0
        AND R0,R0,R0
        BRz PARSE4C
        JSR CALLSV
        LDI R0,HANDLENUMREF
        JSRR R0
        JSR CALLLD
        BRnzp PARSE4C
PARSE4A AND R1,R1,#0 ; else if it was just a minus
        ADD R1,R1,#1 ; handle the minus as an operator. minus has index 1.
        LD R0,PASSNO ; dont call handlers in pass 0
        AND R0,R0,R0
        BRz PARSE4C
        JSR CALLSV
        LDI R0,HANDLEOPREF
        JSRR R0
        JSR CALLLD
PARSE4C AND R2,R2,#0
        ADD R2,R2,x1 ; reset state to ready-state
        ADD R4,R4,#-1
        BRz PARSE9 ; go to PARSE9 if this was only a spacebar
        LD R0,PASSNO
        AND R0,R0,R0
        BRz PARSE4D
        JSR CALLSV
        LDI R0,HANDLEENDREF
        JSRR R0
        JSR CALLLD
        BRnzp RESET ; student should have jumped to reset, but i suppose we do it for them if they return.
PARSE4D AND R0,R0,#0
        ADD R0,R0,#1
        ST R0,PASSNO
        BRnzp PSTART

PARSE9  ST R2,PSTATE ; end of the case-select structure
        ST R3,PNUMSUM

        BRnzp PNEXT ; branch back up to look at next character in buffer

PASSNO  .FILL #0
PROMPTS .STRINGZ ">> "
PSTATE  .BLKW #1
PNUMSUM .BLKW #1        
PCHARS  .STRINGZ "0123456789-+*/ \n\r"
DEL     .FILL #-127 ; negative ASCII DEL
SPACE   .FILL #32
PCOUNTS .FILL #10
        .FILL #1
        .FILL #3
        .FILL #1
        .FILL #2

PRSERR  LEA R0,ERR0
        PUTS
        BRnzp RESET
ERR0    .STRINGZ "ERR0: Parse error.\n"

PRNTNUML BRnzp PRNTNUM ; needed otherwise the offset would be too big

BUFFLEN .FILL #80
STRBUFF .BLKW #80
        .FILL #0 ; extra null char after string

CALLSV  ST R0,CLSVR0
        ST R1,CLSVR1
        ST R2,CLSVR2
        ST R3,CLSVR3
        ST R4,CLSVR4
        ST R5,CLSVR5
        ST R6,CLSVR6
        RET
CLSVR0  .BLKW #1
CLSVR1  .BLKW #1
CLSVR2  .BLKW #1
CLSVR3  .BLKW #1
CLSVR4  .BLKW #1
CLSVR5  .BLKW #1
CLSVR6  .BLKW #1
CALLLD  LD R0,CLSVR0
        LD R1,CLSVR1
        LD R2,CLSVR2
        LD R3,CLSVR3
        LD R4,CLSVR4
        LD R5,CLSVR5
        LD R6,CLSVR6
        RET

 ; print a numeral in R0, including minus sign for negatives
PRNTNUM ST R0,PRNTRSV
        LEA R0,PRNTRSV
        STR R1,R0,#1
        STR R2,R0,#2
        STR R3,R0,#3
        STR R4,R0,#4
        STR R5,R0,#5
        STR R6,R0,#6
        STR R7,R0,#7
        LD R1,PRNTRSV ; put number to write in R1
        BRzp PRNT0
        LD R0,MINUS
        OUT
        NOT R1,R1
        ADD R1,R1,#1
PRNT0   AND R5,R5,#0  
        ADD R5,R5,#1 ; prevent leading zeros. 1 means dont print 0's; 0 means do.
        LEA R2,DEC
PRNT1   LDR R3,R2,#0
        BRz PRNT9
        ADD R2,R2,#1
        AND R4,R4,#0
PRNT2   ADD R1,R3,R1
        BRn PRNT3
        ADD R4,R4,#1
        AND R5,R5,#0 ; now we can write zeros
        BRnzp PRNT2
PRNT3   NOT R3,R3
        ADD R3,R3,#1
        ADD R1,R3,R1 ; we went negative before branching, so add it back
        ADD R4,R4,#0
        BRp PRNT4
        ADD R0,R3,#-1 ; make sure we print if we're on the last digit
        BRz PRNT4
        ADD R5,R5,#0
        BRp PRNT1
PRNT4   LD R3,DOFFSET
        ADD R0,R3,R4
        OUT
        BRnzp PRNT1
PRNT9   LEA R0,PRNTRSV ; finish up
        LDR R1,R0,#1
        LDR R2,R0,#2
        LDR R3,R0,#3
        LDR R4,R0,#4
        LDR R5,R0,#5
        LDR R6,R0,#6
        LDR R7,R0,#7
        LD R0,PRNTRSV
        RET
MINUS   .FILL #45 ; '-'
DOFFSET .FILL #48 ; '0'
DEC     .FILL #-10000 ; largest, since 2^15=32768
        .FILL #-1000
        .FILL #-100
        .FILL #-10
        .FILL #-1
        .FILL #0
PRNTRSV .BLKW #8

        .END
