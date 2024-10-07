.include "m328PBdef.inc"
.def temp = r16
.def train = r17
    
init_stack:
    ldi temp, low(RAMEND)	; initialize stack pointer
    out SPL, temp
    ldi temp, high(RAMEND)
    out SPH, temp
    
init_ports:
    ser temp
    out DDRD, temp		; PORTD -> output
    
init:
    set				; Set the T flag
    ldi train, 0x01		; Initialize train at LSB
    ldi r24, low(1000)
    ldi r25, high(1000)		; Prepare 1sec delay
    
output:
    out PORTD, train
    rcall wait_x_msec		; wait 1 sec
    
start:
    brbc 6, right		; if T == 0 then go right
    
left:
    clc				; clear carry flag
    rol train			; rotate left -> move train to the left
    brcc output			; check if train reached the end (carry overflow)
    clt				; Clear the T flag
    ldi train, 0x80		; prepare train for going right
    jmp output
    
right:
    clc				; clear carry flag
    ror train			; rotate right -> move train to the right
    brcc output			; check if train reached the end (carry overflow)
    set
    ldi train, 0x01		; prepare train for going left
    jmp output
    
wait_x_msec:
    push r24			; 2 cycles (0.125 usec)
    push r25			; 2 cycles (0.125 usec)
    
l1: rcall wait_1_msec
    sbiw r24, 1			; 2 cycles (0.125 usec)
    brne l1			; 2 cycles - worst case (1/2 cycles) thus 0.125 usec
    
    pop r25			; 2 cycles (0.125 usec)
    pop r24			; 2 cycles (0.125 usec)
    ret				; 4 cycles (0.25 usec)
    
wait_1_msec:
    push r26			; 2 cycles (0.125 usec)
    push r27			; 2 cycles (0.125 usec)
    
    ldi r26, low(3996)		; 1 cycle (0.0625 usec)
    ldi r27, high(3996)		; 1 cycle (0.0625 usec)
    
l2: sbiw r26, 1			; 2 cycles (0.125 usec)
    brne l2			; 2 cycles (0.125 usec)
    
    pop r27			; 2 cycles (0.125 usec)
    pop r26			; 2 cycles (0.125 usec)
    ret				; 4 cycles (0.25 usec)

