.include "m328PBdef.inc"
.def temp = r16
.def x_low = r24
.def x_high = r25
.def temp_low = r26
.def temp_high = r27
.def train = r17
.equ delay_ms = 1000
    
init_stack:
    ldi temp, low(RAMEND)	; initialize stack pointer
    out SPL, temp
    ldi temp, high(RAMEND)
    out SPH, temp
    
init_ports:
    ser temp
    out DDRD, temp		; PORTD -> output
    
; T == 0 -> train goes right, T == 1 -> train goes left
init:
    set				; Set the T flag
    ldi train, 0x01		; Initialize train at LSB
    ldi x_low, low(delay_ms)
    ldi x_high, high(delay_ms)	; Prepare 1sec delay
    
output:
    out PORTD, train
    rcall wait_x_msec		; wait 1 sec
    
start:
    brbc 6, right		; if T == 0 then go right
    
left:
    clc				; clear carry flag
    rol train			; rotate left -> move train to the left
    brcc output	    ; check if train reached the end (carry overflow)
    clt				; Clear the T flag
    ldi train, 0x80		; prepare train for going right
    jmp output
    
right:
    clc				; clear carry flag
    ror train		; rotate right -> move train to the right
    brcc output	  ; check if train reached the end (carry overflow)
    set
    ldi train, 0x01		; prepare train for going left
    jmp output
    
wait_x_msec:
    push x_low			
    push x_high			
    push temp_low		
    push temp_high

check:    
    sbiw x_low, 1		
    brne outer_loop		

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
    ldi temp_low, low(3998)	
    ldi temp_high, high(3998)	
inner_loop:	
    sbiw temp_low, 1		
    brne inner_loop		
    nop

    rjmp check			

epilogue:
    pop temp_high		
    pop temp_low		
    pop x_high			
    pop x_low			
    ret				