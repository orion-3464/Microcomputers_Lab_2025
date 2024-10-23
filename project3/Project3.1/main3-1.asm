.include "m328PBdef.inc"

.def temp = r16
.def DC_VALUE = r17
.def index = r18
.def x_low = r24
.def x_high = r25
.def temp_low = r26
.def temp_high = r27
.equ delay_ms = 10


; assembler uses word aligned address thus if PWM_values is stored at address 0x04 in words 
; then it is stored at adsress 0x08 in bytes
PWM_values:
.db 0x05, 0x1A, 0x2E, 0x42, 0x57, 0x6B, 0x80, 0x94, 0xA8, 0xBD, 0xD1, 0xE6, 0xFA, 0x00 ; adding extra 0x00 for memory alignement

reset:
    ; TMR1 Configuration
    ldi temp, (1 << CS12) | (1 << WGM12) 
    sts TCCR1B, temp
    ldi temp, (1 << WGM10) | (1 << COM1A1)
    sts TCCR1A, temp

    ; Init Stack
    ldi temp, low(RAMEND)
    out SPL, temp
    ldi temp, high(RAMEND)
    out SPH, temp

    ; Init PORTD as Input
    clr temp
    out DDRD, temp 
    
    ; Init PORTB as output
    ser temp
    out DDRB, temp	;to output the PWM to PB1

    ; Loading "PWM_values" address in bytes, at register Z
    ldi ZH, high(PWM_values*2)
    ldi ZL, low(PWM_values*2)


main:
    ; init delay value
    ldi x_low, low(delay_ms)	
    ldi x_high, high(delay_ms)	

    ; init DC_VALUE
    ldi index, 0x06
    adiw ZL, 0x06
    
    clr temp
    lpm DC_VALUE, Z

    sts OCR1AH, temp
    sts OCR1AL, DC_VALUE

read:
    sbis PIND, 3		; if PD3 is pressed increase
    rjmp increase
    sbis PIND, 4		; if PD4 is pressed decrease
    rjmp decrease

    rjmp read		; if nothing pressed output same as before
    
decrease:
    rcall wait_x_msec
    sbis PIND,4	    ; this is to assure that one press of PD4 won't cause 
		    ; multiple jumps to decrease 
    rjmp decrease
    cpi index, 0x00
    breq read ; saturation
    dec index
    sbiw ZL, 1
    lpm DC_VALUE, Z
    sts OCR1AL, DC_VALUE
    rjmp read 
    
increase:
    rcall wait_x_msec
    sbis PIND,3	    ; this is to assure that one press of PD3 won't cause 
		    ; multiple jumps to increase
    rjmp increase
    cpi index, 0x0C
    breq read		; saturation
    inc index
    adiw ZL, 1
    lpm DC_VALUE, Z
    sts OCR1AL, DC_VALUE
    rjmp read
    

wait_x_msec:
    push x_low			; 2 cycles (0.125 usec)
    push x_high			; 2 cycles (0.125 usec)
    push temp_low		; 2 cycles (0.125 usec)
    push temp_high

check:    
    sbiw x_low, 1		; 2 cycles
    brne outer_loop		; 2 cycles *

msec1:
    ldi temp_low, low(3992)
    ldi temp_high, high(3992)
msec1_loop:
    sbiw temp_low, 1
    brne msec1_loop

    nop
    nop
    nop
    rjmp epilogue

outer_loop:
    ldi temp_low, low(3998)	; 1 cycle
    ldi temp_high, high(3998)	; 1 cycle
inner_loop:	
    sbiw temp_low, 1		; 2 cycles
    brne inner_loop		; 2 cycles *
    nop

    rjmp check			; 2 cycles *

epilogue:
    pop temp_high		; 2 cycles (0.125 usec)
    pop temp_low		; 2 cycles (0.125 usec)
    pop x_high			; 2 cycles (0.125 usec)
    pop x_low			; 2 cycles (0.125 usec)
    ret				; 4 cycles (0.25 usec)

