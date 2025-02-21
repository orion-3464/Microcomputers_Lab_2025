.include "m328PBdef.inc"    ; ATmega328PB microcontroller definitions

.equ FOSC_MHZ=16	    ; Microcontroller operating frequency in MHz

.equ DEL_mS=500		    ; Delay in mS (valid number from 1 to 4095)

.equ DEL_NU=FOSC_MHZ*DEL_mS ; delay_mS routine: (1000*DEL_NU+6) cycles

.def counter = r16	; for counting number of interrupts
.def temp = r20
.def output = r22

; Init jump vector for INT1
.org 0x0
    rjmp begin
.org 0x4
    rjmp ISR1

begin:
; Init Stack Pointer
    ldi temp, LOW(RAMEND)
    out SPL, temp
    ldi temp, HIGH(RAMEND)
    out SPH, temp

; Init PORTD as input
    clr temp
    out DDRD, temp

; Init PORTB and PORTC as output
    ser temp
    out DDRB, temp
    out DDRC, temp

; Init interrupts
    ldi temp, (1<<ISC11)|(1<<ISC10)
    sts EICRA, temp	; Interrupt in rising edge

    ldi temp, (1<<INT1)
    out EIMSK, temp	; Enabling INT1     
    
    sei			; Enabling all interrupts

; Init interrupts' counter
    clr counter
    out PORTC, counter

loop1:
    clr output
loop2:
    sbis PIND, 5
    cli			    ; if PD5 = 1 then don't cli
    ldi temp, (1 << INTF1)
    out EIFR, temp	    ; clearing the INT1 flag all the interrupts that occured while PD5 was pressed 

    sbic PIND, 5	    ; if PD5 = 0 then cli will be excecuted 
    sei

    out PORTB, output

    ldi r24, low(DEL_NU)   ;	
    ldi r25, high(DEL_NU)  ; Set delay (number of cycles)
    rcall delay_mS	   ; 

    inc output

    cpi output, 16		   ; compare r26 with 16
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


ISR1:
    push r24
    push r25

   sparking:
    ldi temp, (1 << INTF1)
    out EIFR, temp	    ; clearing the INT1 flag

    ldi r24, LOW(80)
    ldi r25, HIGH(80)	    ; 80 = 16*5 for 5ms delay
    rcall delay_mS	    ; delay 10ms

    sbic EIFR, 1	    ; if interrupt occured go clear the flag
    rjmp sparking

    inc counter
    sbrc counter,7	    ; if counter hasn't reached 64 don't clear 
    clr counter
    out PORTC, counter
    
    pop r25
    pop r24

    reti

