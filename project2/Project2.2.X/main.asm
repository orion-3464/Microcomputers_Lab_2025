.include "m328PBdef.inc"    ; ATmega328PB microcontroller definitions

.equ FOSC_MHZ=16	    ; Microcontroller operating frequency in MHz

.equ DEL_mS=2000		    ; Delay in mS (valid number from 1 to 4095)
 
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

;Init Stack Pointer
    ldi r24, LOW(RAMEND)
    out SPL, r24
    ldi r24, HIGH(RAMEND)
    out SPH, r24

; Init PORTC as output and PORTB as input
    ser r26
    out DDRC, r26
    clr r26
    out DDRB, r26

; Counter code
loop1:
    clr r26
loop2:
    out PORTC, r26
    
    ldi r24, low(DEL_NU)   ;	
    ldi r25, high(DEL_NU)  ; Set delay (number of cycles)
    rcall delay_mS	   ; 

    inc r26

    cpi r26, 32		   ; compare r26 with 32
    breq loop1
    rjmp loop2


; Interrupt Service Routine (The interrupt calls the delay routine)
ISR0:
    push r17
    push r18
    push r19
    push r20

    in r17, PINB	    ; r17 stores buttons' values
    ser r18		    ; r18 stores the output of the service routine
    ldi r19, 4		    ; counts 4 iterations
    ldi r20, 0b00001111	    ; r20 is a mask
    and r17, r20	    ; 4 LSBs of the output

count_buttons:
    ror r17
    brcs check		    ; if carry=1, continue counting
    lsl r18		    ; if carry=0, count one more button
check:
    dec r19
    brne count_buttons	    ; continue counting until 4

    com r18
    out PORTC, r18	    ; light the correct amount of LEDs
    rcall delay_mS

    pop r20
    pop r19
    pop r18
    pop r17
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