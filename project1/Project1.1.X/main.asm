.include "m328PBdef.inc"
.def temp = r16
.def x_low = r24
.def x_high = r25
.def temp_low = r26
.def temp_high = r27
.equ delay_ms = 1000
    
init_stack:
    ldi temp, low(RAMEND)	; initialize stack pointer
    out SPL, temp
    ldi temp, high(RAMEND)
    out SPH, temp

main:
    ldi x_low, low(delay_ms)	; load x = r25:r24 with 1000
    ldi x_high, high(delay_ms)	; x = 1000
    rcall wait_x_msec		; delay x msec (here x = 1000)
    rjmp end
        
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
    
end:
    rjmp end
