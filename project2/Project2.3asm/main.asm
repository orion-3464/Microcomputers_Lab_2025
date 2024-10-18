.include "m328PBdef.inc"

.equ FOSC_MHZ = 16	    ; Microcontroller operating frequency in MHz

.equ DEL_mS = 2500	    ; Delay in mS (valid number from 1 to 4095)

.equ DEL_5 = FOSC_MHZ*DEL_mS ; delay_mS routine: (1000*DEL_NU+6) cycles

.equ DEL_05 = FOSC_MHZ*500

.def temp = r16

.def flag = r18

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

; Init PORTB as output
    ser temp
    out DDRB, temp

; Init interrupts
    ldi temp, (1<<ISC11)|(1<<ISC10)
    sts EICRA, temp	; Interrupt in rising edge

    ldi temp, (1<<INT1)
    out EIMSK, temp	; Enabling INT1     
    
    clr flag
    
    sei			; Enabling all interrupts

main:
    clr temp
    out PORTB, temp
    rjmp main


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
    inc flag
    cpi flag, 0xFF
    brne overflow_free
    ldi flag, 0x02	; for preventing overflow of the flag register

  overflow_free:
    sei
    
    push r24
    push r25
    
    cpi flag, 0x01
    brne all_leds

  one_led:
    ldi temp, 0x01
    out PORTB, temp	   ; opening PB0 led
    
    ; Delay 5s
    ldi r24, LOW(DEL_5)
    ldi r25, HIGH(DEL_5)
    rcall delay_mS
    
    ldi r24, LOW(DEL_5)
    ldi r25, HIGH(DEL_5)
    rcall delay_mS
    
    rjmp epilogue

  all_leds:
    pop temp
    ser temp
    out PORTB, temp
    
    ldi r24, LOW(DEL_05)
    ldi r25, HIGH(DEL_05)
    rcall delay_mS
    
    dec flag
    
    rjmp one_led

  epilogue:

    clr flag

    pop r25
    pop r24

    reti

