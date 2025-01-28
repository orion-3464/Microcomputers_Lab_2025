write_2_nibbles:
    push r24		; save r24(LCD_Data)
    in r25 ,PIND	; read PIND
    andi r25 ,0x0f	;
    andi r24 ,0xf0	; r24[3:0] Holds previus PORTD[3:0]
    add r24 ,r25	; r24[7:4] <-- LCD_Data_High_Byte
    out PORTD ,r24	;
    sbi PORTD ,PD3	; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    pop r24		; Recover r24(LCD_Data)
    swap r24		;
    andi r24 ,0xf0	; r24[3:0] Holds previus PORTD[3:0]
    add r24 ,r25	; r24[7:4] <-- LCD_Data_Low_Byte
    out PORTD ,r24
    sbi PORTD ,PD3	; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    ret


lcd_data:
    sbi PORTD ,PD2 ; LCD_RS=1(PD2=1), Data
    rcall write_2_nibbles ; send data
    ldi r24 ,250 ;
    ldi r25 ,0 ; Wait 250uSec
    rcall wait_usec
    ret


lcd_command:
    cbi PORTD ,PD2 ; LCD_RS=0(PD2=0), Instruction
    rcall write_2_nibbles ; send Instruction
    ldi r24 ,250 ;
    ldi r25 ,0 ; Wait 250uSec
    rcall wait_usec
    ret


lcd_clear_display:
    ldi r24 ,0x01 ; clear display command
    rcall lcd_command
    ldi r24 ,low(5) ;
    ldi r25 ,high(5) ; Wait 5 mSec
    rcall wait_msec ;
    ret


lcd_init:
    ldi r24 ,low(200) ;
    ldi r25 ,high(200) ; Wait 200 mSec
    rcall wait_msec ;
    ldi r24 ,0x30 ; command to switch to 8 bit mode
    out PORTD ,r24 ;
    sbi PORTD ,PD3 ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    ldi r24 ,250 ;
    ldi r25 ,0 ; Wait 250uSec
    rcall wait_usec ;
    ldi r24 ,0x30 ; command to switch to 8 bit mode
    out PORTD ,r24 ;
    sbi PORTD ,PD3 ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    ldi r24 ,250 ;
    ldi r25 ,0 ; Wait 250uSec
    rcall wait_usec ;
    ldi r24 ,0x30 ; command to switch to 8 bit mode
    out PORTD ,r24 ;
    sbi PORTD ,PD3 ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    ldi r24 ,250 ;
    ldi r25 ,0 ; Wait 250uSec
    rcall wait_usec
    ldi r24 ,0x20 ; command to switch to 4 bit mode
    out PORTD ,r24
    sbi PORTD ,PD3 ; Enable Pulse
    nop
    nop
    cbi PORTD ,PD3
    ldi r24 ,250 ;
    ldi r25 ,0 ; Wait 250uSec
    rcall wait_usec
    ldi r24 ,0x28 ; 5x8 dots, 2 lines
    rcall lcd_command
    ldi r24 ,0x0c ; dislay on, cursor off
    rcall lcd_command
    rcall lcd_clear_display
    ldi r24 ,0x06 ; Increase address, no display shift
    rcall lcd_command ;
    ret
