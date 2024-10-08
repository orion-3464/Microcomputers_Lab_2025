.include "m328PBdef.inc"

start:
    ldi r16, 0x01    ; Initialize the bayonet to the LSB (bit 0)
    out PORTD, r16   ; Output initial value to PORTD
    clt              ; Clear the T flag (left shift direction)

main_loop:
    brtc shift_left  ; If T bit is cleared (shift left), branch to shift_left

shift_right:
    lsr r16          ; Logical shift right
    out PORTD, r16   ; Output the shifted value to PORTD
    brcc reached_lsb  ; If carry is set (bit 0 reached), branch to reached_lsb
    rcall delay_1sec ; Call 1-second delay
    rjmp main_loop   ; Loop back to main loop

shift_left:
    lsl r16          ; Logical shift left
    out PORTD, r16   ; Output the shifted value to PORTD
    brcs reached_msb ; If carry is clear (not MSB), branch to reached_msb
    rcall delay_1sec ; Call 1-second delay
    rjmp main_loop   ; Loop back to main loop

reached_msb:
    set
    ldi r16,0x80; Set the T flag to indicate direction change to right
    rcall delay_2sec ; Call 2-second delay at MSB
    rjmp main_loop   ; Loop back to main loop

reached_lsb:
    clt         
    ldi r16,0x01; Clear the T flag to indicate direction change to left
    rcall delay_2sec ; Call 2-second delay at LSB
    rjmp main_loop   ; Loop back to main loop

; ======================
; Delay functions
; ======================
delay_1sec:
    ; Call wait_x_msec with 1000 ms for 1-second delay
    ldi r24, LOW(1000)  ; Load low byte of 1000 ms into r24
    ldi r25, HIGH(1000) ; Load high byte of 1000 ms into r25
    rcall wait_x_msec   ; Call delay routine
    ret

delay_2sec:
    ; Call wait_x_msec with 2000 ms for 2-second delay
    ldi r24, LOW(2000)  ; Load low byte of 2000 ms into r24
    ldi r25, HIGH(2000) ; Load high byte of 2000 ms into r25
    rcall wait_x_msec   ; Call delay routine
    ret

; ======================
; Custom wait_x_msec delay routine
; ======================
wait_x_msec:
    push r24
    push r25          ; Save r24 and r25 to the stack to preserve their values
delay_loop:
    ldi r18, 42      ; Loop count for ~1ms delay
inner_loop:
    ldi r19, 127
delay_1ms:
    dec r19
    brne delay_1ms
    dec r18
    brne inner_loop
    sbiw r24, 1      ; Decrement the delay counter
    brne delay_loop
    pop r25
    pop r24          ; Restore r24 and r25 from the stack
    ret

