.include "m328PBdef.inc"    ; ATmega328PB microcontroller definitions

.equ FOSC_MHZ=16	    ; Microcontroller operating frequency in MHz

.equ DEL_mS=500		    ; Delay in mS (valid number from 1 to 4095)
 
.equ DEL_NU=FOSC_MHZ*DEL_mS ; delay_mS routine: (1000*DEL_NU+6) cycles

;Init Stack Pointer
    ldi r24, LOW(RAMEND)
    out SPL, r24
    ldi r24, HIGH(RAMEND)
    out SPH, r24

; Init PORTB as output
    ser r26
    out DDRB, r26


loop1:
    clr r26
loop2:
    out PORTB, r26
    
    ldi r24, low(DEL_NU)   ;	
    ldi r25, high(DEL_NU)  ; Set delay (number of cycles)
    rcall delay_mS	   ; 

    inc r26

    cpi r26, 16		   ; compare r26 with 16
    breq loop1
    rjmp loop2

; delay of 1000*F1+6 cycles (almost equal to 1000*F1 cycles)
delay_mS:

; total dalay of next 4 instruction group  = 1+(249*4-1) = 996 cycles
    ldi r23, 249	   ; (1 cycle)
loop_inn:
    dec r23		   ; 1 cycle
    nop
    brne loop_inn	   ; 1 or 2 cycles

    sbiw r24, 1		   ; 2 cycles
    brne delay_mS	   ; 1 or 2 cycles

    ret			   ; 4 cycles
