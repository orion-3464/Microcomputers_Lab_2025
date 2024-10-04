.include "m328PBdef.inc"
.def temp=r16
    
init_stack:
    ldi temp, low(RAMEND)	; initialize stack pointer
    out SPL, temp
    ldi temp, high(RAMEND)
    out SPH, temp

init_ports:
    ser temp	; temp = 0b11111111
    out PORTB, temp	; PORTB's bits are set as output
    
main:
    clr temp	; temp = 0b00000000
    out PORTB, temp	; open all PORTB's LEDs
    
    ldi r24, low(1000)	; load x = r25:r24 with 1000
    ldi r25, high(1000)	; x = 1000, so the led blinks every one second
    rcall wait_x_msec	; delay x msec (here x = 1000)
    ser temp	; temp = 0b11111111
    out PORTB, temp	; close all PORTB's LEDs
    rcall wait_x_msec	; delay x msec (here x = 1000)

; minimum possible delay is 0.825 ?sec
wait_x_msec:
    push r24		; 2 cycles (0.125 ?sec)
    push r25		; 2 cycles (0.125 ?sec)
    sbiw r24, 0.001	; correction according to default commands aka push pop ret, (1 ?sec = 0.001 msec in total)
    sbiw r24, 1		; 2 cycles (0.125 ?sec)
    brne r24		; 2 cycles - worst case (1/2 cycles) thus 0.125 ?sec
    pop r25		; 2 cycles (0.125 ?sec)
    pop r24		; 2 cycles (0.125 ?sec)
    ret			; 4 cycles (0.25 ?sec)