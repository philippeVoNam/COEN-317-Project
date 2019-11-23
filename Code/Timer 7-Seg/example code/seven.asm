;*****************************************************************
;* This stationery serves as the framework for a                 *
;* user application (single file, absolute assembly application) *
;* For a more comprehensive program that                         *
;* demonstrates the more advanced functionality of this          *
;* processor, please see the demonstration applications          *
;* located in the examples subdirectory of the                   *
;* Freescale CodeWarrior for the HC12 Program directory          *
;*****************************************************************

; export symbols
            XDEF Entry, _Startup            ; export 'Entry' symbol
            ABSENTRY Entry        ; for absolute assembly: mark this as application entry point
   
; Include derivative-specific definitions 
		INCLUDE 'derivative.inc' 

ROMStart    EQU  $4000  ; absolute address to place my code/constant data

; variable/data section
            ORG RAMStart

; Insert here your data definition.
sevsegdata dc.b  $BF, $86, $DB, $CF, $E6, $ED, $FD, $87, $FF, $EF, $F7, $FC, $B9, $DE, $F9, $F1

; code section
            ORG   ROMStart

Entry:
_Startup:  
            LDS   #RAMEnd+1       ; initialize the stack pointer

; Dragon12-Light Board:
            ; Port A: Connected to Keypad. Configure it as input (DDRA).
            ; Port B: Connected to LEDs. Configure it as output (DDRB). Also set PJ1 to 0, and set DDRJ as output ($FF)
            ;         Also connected to all four 7-seg anodes (PB0-PB7: A,B, ...,G, Dec. Point). Positive logic for both LEDs and 7-seg
            ;
            ; Port P: The last 4 bits (PP3-PP0) are connected to the cathodes of the 7-seg LEDs. Configure those 4 bits as output (DDRP)
            ;         7-seg LEDs cathodes: Negative logic. A '0' is required to turn on a particular 7-seg display
            ; Port H: Connected to DIPSwitches (H7-H0) and Pushbuttons (H3-H0). Configure it as input (DDRH)
            ;         DIPSwitches (H7-H0): Positive logic. Up: 1, Down: 0
            ;         Pushbuttons (H3-H0): When pressed, they send a 0 and take precedence over the DIPswitches.
            ;                              When not pressed, only the DIPswitches are paid attention to..
            ; Port K: Connected to LCD. Configure it as output
            
            ; Port K (3..0), Port P (3..0), Port T, Port A,B, Port H, Port E: Can be used as general I/O (see headers)
                        
            ; I/O Configuration
            movb #$F0, DDRA          ; set Port A (7-4) to be outputs, Port A (3-0) to be inputs
            movb #$FF, DDRB          ; set Port B to be all output
            movb #$FF, DDRP          ; set Port P (3 to 0) to be all output (the others, I don't care)
            movb #$00, DDRH          ; set Port H to be all input
            movb #$FF, DDRK          ; set Port K to be all output
            

            ; I/O usage: 7-seg and DIP switches
            ;  pt  g   f   e   d   c   b   a
            ; PB7 PB6 PB5 PB4 PB3 PB2 PB1 PB0
            ;  1   0   1   1   1   1   1   1   =  0xBF = '0' in the 7-segment display
            ;  1   0   0   0   0   1   1   0   =  0x86 = '1' in the 7-segment display
            ;  1   1   0   1   1   0   1   1   =  0xDB = '2' in the 7-segment display
            ;  1   1   0   0   1   1   1   1   =  0xCF = '3' in the 7-segment display
            ;  1   1   1   0   0   1   1   0   =  0xE6 = '4' in the 7-segment display
            ;  1   1   1   0   1   1   0   1   =  0xED = '5' in the 7-segment display
            ;  1   1   1   1   1   1   0   1   =  0xFD = '6' in the 7-segment display
            ;  1   0   0   0   0   1   1   1   =  0x87 = '7' in the 7-segment display
            ;  1   1   1   1   1   1   1   1   =  0xFF = '8' in the 7-segment display
            ;  1   1   1   0   1   1   1   1   =  0xEF = '9' in the 7-segment display
            ;  1   1   1   1   0   1   1   1   =  0xF7 = 'A' in the 7-segment display
            ;  1   1   1   1   1   1   0   0   =  0xFC = 'B' in the 7-segment display
            ;  1   0   1   1   1   0   0   1   =  0xB9 = 'C' in the 7-segment display
            ;  1   1   0   1   1   1   1   0   =  0xDE = 'D' in the 7-segment display
            ;  1   1   1   1   1   0   0   1   =  0xF9 = 'E' in the 7-segment display
            ;  1   1   1   1   0   0   0   1   =  0xF1 = 'F' in the 7-segment display
            
            ldx #sevsegdata; X <- sevsegdata (X grabs the address)            
            movb #$00, PTP   ; All four 7-seg displays cathodes are set to 0 (i.e., they are turned on)
            
            
showDIPSW:  
            tba ; A <= B. B receives the parameter.
            ;ldaa #10        ; Data from DIP switches appear in A. Note that PTH(3..0) are also the pushbuttons
                            ; Note that data on the 7-seg displays is only valid for DIP switches 3 to 0. (from 0 to F)
            anda #$0F       ; Only the 4 LSBs are considered
            ldab A, X       ; B <- [A+sevsegdata].
                            ; If A=$00, then, B = [sevsegdata] = $BF
                            ; If A=$03, then, B = [$03+sevsegdata] = $CF
            stab PORTB      ; Data from A is loaded on all the four 7-seg displays
            bra showDIPSW



     
         
         
         
            
