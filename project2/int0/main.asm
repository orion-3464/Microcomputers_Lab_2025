.include "m328PBdef.inc"

.equ FOSC_MHZ=16	    ; Microcontroller operating frequency in MHz

.equ DEL_mS=500		    ; Delay in mS (valid number from 1 to 4095)
 
.equ DEL_NU=FOSC_MHZ*DEL_mS ; delay_mS routine: (1000*DEL_NU+6) cycles

.org 0x0
rjmp reset
.org 0x2
rjmp ISR0

; activate INT0 interrupt
reset:
; Interrupt on rising edge of INT0 pin
ldi r24, (1 << ISC01) | (1 << ISC00)
sts EICRA, r24

; Enable the INT0 interrupt (PD2)
ldi r24, (1 << INT0)
out EIMSK, r24

sei			    ; Sets the Global Interrupt Flag

; Interrupt Service Routine (The interrupt calls the delay routine)
ISR0:
    push r25		    ;
    push r24		    ;
    in r24, SREG	    ; Save r24, r25, SREG
    push r24		    ; 

    ser r24
    out PORTB, r24

    ldi r24, low(16*500)    ;
    ldi r25, high(16*500)  ; Set delay (number of cycles)
    rcall delay_mS


    pop r24		    ;
    out SREG, r24	    ; Restore r24, r25, SREG
    pop r24		    ;
    pop r25		    ;

    reti

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
