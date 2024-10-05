.include "m328PBdef.inc"
.def temp=r16
    
init_stack:
    ldi temp, low(RAMEND)	; initialize stack pointer
    out SPL, temp
    ldi temp, high(RAMEND)
    out SPH, temp
    
main:
    ldi r24, low(2)		; load x = r25:r24 with 1000
    ldi r25, high(2)		; x = 1000, so the led blinks every one second
    rcall wait_x_msec		; delay x msec (here x = 1000)
    rjmp end
    
; minimum possible delay is 0.825 usec
wait_x_msec:
    push r24			; 2 cycles (0.125 usec)
    push r25			; 2 cycles (0.125 usec)
    
l:  sbiw r24, 1		; 2 cycles (0.125 usec)
    rcall wait_1_msec
    brne l			; 2 cycles - worst case (1/2 cycles) thus 0.125 usec
    
    pop r25			; 2 cycles (0.125 usec)
    pop r24			; 2 cycles (0.125 usec)
    ret				; 4 cycles (0.25 usec)
    
wait_1_msec:
    push r26			; 2 cycles (0.125 usec)
    push r27			; 2 cycles (0.125 usec)
    
    ldi r26, low(244)		; 1 cycle (0.0625 usec)
    ldi r27, high(244)		; 1 cycle (0.0625 usec)
    
l2: sbiw r26, 1			; 2 cycles (0.125 usec)
    brne l2			; 2 cycles (0.125 usec)
    
    pop r27			; 2 cycles (0.125 usec)
    pop r26			; 2 cycles (0.125 usec)
    ret				; 4 cycles (0.25 usec)
    
end: