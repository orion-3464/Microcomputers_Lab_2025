.include "m328PBdef.inc"
    
.def temp1 = r16  
.def temp2 = r17    
    
.equ PD0 = 0
.equ PD1 = 1
.equ PD2 = 2
.equ PD3 = 3
.equ PD4 = 4
.equ PD5 = 5
.equ PD6 = 6
.equ PD7 = 7
    
.org 0x00
    rjmp Init

.org 0x2A
    rjmp ADC_read
    
; --------------------------------------------- INITIALIZATIONS ------------------------------------------------------ ;    
Init:
; Init Stack Pointer
    ldi temp1, low(RAMEND)
    out SPL, temp1
    ldi temp1, high(RAMEND)
    out SPH, temp1
    
; Init PORTC as Input
    clr temp1
    out DDRC, temp1
    
; Init PORTD as Output
    ser temp1
    out DDRD, temp1
    
; Init ADMUX
    ldi temp1, (1 << REFS0) | (1 << MUX0)
    sts ADMUX, temp1
    
;Init ADCSRA
    ldi temp1, (1 << ADEN) | (1 << ADIE)
    sts ADCSRA, temp1

; Init LCD
    rcall lcd_init
    
    ldi r24, low(100)
    ldi r25, high(100)	    ; delay 100 mS
    rcall wait_msec
    
    rcall lcd_clear_display
    
    sei 

; -------------------------------------------------------------- MAIN ----------------------------------------------------------- ;
Main: 
    lds temp1, ADCSRA
    ori temp1, (1 << ADSC)
    sts ADCSRA, temp1
    
    rjmp Main
    

; -------------------------------------------- ADC INTERRUPT ROUTINE --------------------------------------------------------- ;
ADC_read:
    lds YL, ADCL
    lds YH, ADCH
    
    ; ------ Calculate Vin ------- ;
    mov XH, YH	    ; Y contains the ADC value
    mov XL, YL
     
    clc		    ; clear C flag
    rol XL	    
    rol XH
    
    clc
    rol XL	    
    rol XH	
    
    add XL, YL
    adc XH, YH
    
    ; X contains the value of Vin
    ; integer part is at XH[7:2] bits
    ; decimal part is at XH[1:0]:XL[7:0] bits
    
    ; ------ DISPLAY ----- ;
    rcall lcd_clear_display
    
    ldi r24, low(10)
    ldi r25, high(10)	; Delay
    rcall wait_msec 
    
    ; ------ for the integer part ------- ;
    mov r24, XH		    ;r24 is the register used by lcd_data
    andi r24, 0b11111100
    lsr r24
    lsr r24
    ldi temp2, 48	    ; 48 is the offset of numbers in lcd screen
    add r24, temp2
    rcall lcd_data
    
    ldi r24, low(10)
    ldi r25, high(10) ; delay 100 mS
    rcall wait_msec
    
    ldi r24, '.'
    rcall lcd_data
    
    ldi r24, low(10)
    ldi r25, high(10)	; Delay
    rcall wait_msec 
    
    
  
    ; ------ for the decimal part ------ ;
    andi XH, 0b00000011	    ; XH:XL now has the decimal part
    
    mov YH, XH
    mov YL, XL	    ; we need a copy in order to calculate multiplication with 10
    
    clc 
    rol XL
    rol XH
    
    clc 
    rol YL
    rol YH
    clc 
    rol YL
    rol YH
    clc 
    rol YL
    rol YH
    
    add XL, YL
    adc XH, YH
    
    mov r24, XH
    andi r24, 0b11111100    ;integer part
    lsr r24
    lsr r24
    ldi temp2, 48
    add r24, temp2
    rcall lcd_data	 ; first decimal digit
    
    ldi r24, low(10)
    ldi r25, high(10)	; Delay
    rcall wait_msec 
    
    andi XH, 0b00000011	    ;subtracting integer part
    mov YH, XH
    mov YL, XL
    
    clc 
    rol XL
    rol XH
    
    clc 
    rol YL
    rol YH
    clc 
    rol YL
    rol YH
    clc 
    rol YL
    rol YH
    
    add XL, YL
    adc XH, YH
    
    mov r24, XH
    andi r24, 0b11111100    ;integer part
    lsr r24
    lsr r24
    ldi temp2, 48
    add r24, temp2
    rcall lcd_data	    ;second decimal digit
    
    ldi r24, low(1000)
    ldi r25, high(1000)	; Delay
    rcall wait_msec
    
    
    reti

    
.include "delays.inc"
.include "lcd.inc"    
