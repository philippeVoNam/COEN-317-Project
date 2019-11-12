; Function implemented in Assembly Code
; Input parameter (1 byte): in Register B
; Output parameter (1 byte): None. However, we use the variable 'ASMData' as the repository of the output value.
;                                  The .c file needs to access this variable 'ASMData'
;                            

; External declaration of data      
;     XREF CData   ; External declaration of a variable. This is a global variable defined in the .c file.
                   ; 
      XDEF asm_mydelay1ms ; Defines the beginning of a particular section (code/data). In this case, it's code
      XDEF asm_mydelay10us
      
      XDEF ASMData1ms ; Defines the beginning of a particular section (code/data). In this case, it's data
      XDEF ASMData10us
      
DataSec: SECTION
ASMData1ms: DS.B 1
ASMData10us: DS.B 1

CodeSec: SECTION

; Program: Delay of 'B; ms:
; If Input Parameter (B) is 0, then ASMData= 0xFF to indicate something is wrong
; If Input Parameter (B) is not 0, then ASMData= 0x00 to indicate nothing is wrong

; Outer loop: It generates a delay of [B]*(1ms+1.25us) + pshx+tba+cmpa+beq+movb+bra+pulx (minimum compared to B ms)
;             , i.e. B ms. B = 1 to 255 ms.      
; Each iteration of the outer loop: 1 ms + ldx + dbne = 1 ms + 2+3 cycles = 1ms + 1.25us)
; Inner loop: It generates a delay of 1 ms.

; 1 ms delay by B
asm_mydelay1ms:  
            pshx        
            tba ; A <= B. B receives the parameter.          
            cmpa #$00
            beq next
            
oloop:      ldx #2400  ; #2400 (24MHz bus speed). Even with no frequency boost, it has been tested that by default 24 MHz is the bus speed.
iloop:      psha      ; 2 cycles
            pula      ; 3 cycles
            nop       ; 1 cycle
            nop       ; 1 cycle
            dbne X,iloop; ;3 cycles
            dbne A, oloop; 3 cycles
            
            movb #$00, ASMData1ms; 4 cycles. Result in ASMData1ms
            bra fini
            
next:       movb #$FF, ASMData1ms; 4 cycles. Result in ASMData1ms
fini:       pulx
            rts    

; 10 us delay by B            
asm_mydelay10us:  
            pshx        
            tba ; A <= B. B receives the parameter.          
            cmpa #$00
            beq unext
            
uoloop:     ldx #24   ; #24 (24MHz bus speed). Even with no frequency boost, it has been tested that by default 24 MHz is the bus speed.
uiloop:     psha      ; 2 cycles
            pula      ; 3 cycles
            nop       ; 1 cycle
            nop       ; 1 cycle
            dbne X,uiloop; ;3 cycles
            dbne A,uoloop; 3 cycles
            
            movb #$00, ASMData10us; 4 cycles. Result in ASMData10us
            bra ufini
            
unext:      movb #$FF, ASMData10us; 4 cycles. Result in ASMData10us
ufini:      pulx
            rts                
            