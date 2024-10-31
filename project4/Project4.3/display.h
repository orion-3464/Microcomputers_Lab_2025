#ifndef DISPLAY_H
#define	DISPLAY_H

#include <avr/io.h>
#include <util/delay.h>

void write_2_nibbles(uint8_t data) {
    uint8_t prev;
    uint8_t current;
    
    prev = PIND & 0x0F;
    current = (data & 0xF0) | prev;
    PORTD = current;
    PORTD |= (1 << PD3);
    PORTD &= ~(1 << PD3);
    
    data <<= 4;
    current = (data & 0xF0) | prev;
    PORTD = current;
    PORTD |= (1 << PD3);
    PORTD &= ~(1 << PD3);
}

void lcd_data(uint8_t byte) {
    PORTD |= (1 << PD2);
    write_2_nibbles(byte);
    _delay_us(250);
}

void lcd_command(uint8_t byte) {
    PORTD &= ~(1 << PD2);
    write_2_nibbles(byte);
    _delay_us(250);
}

void lcd_clear_display() {
    uint8_t command = 0x01;
    lcd_command(command);
    _delay_ms(5);
}

void lcd_init() {
    _delay_ms(200);
    
    PORTD = 0x30;
    PORTD |= (1 << PD3);
    PORTD &= ~(1 << PD3);
    _delay_us(250);
    
    PORTD = 0x30;
    PORTD |= (1 << PD3);
    PORTD &= ~(1 << PD3);
    _delay_us(250);
    
    PORTD = 0x30;
    PORTD |= (1 << PD3);
    PORTD &= ~(1 << PD3);
    _delay_us(250);
    
    PORTD = 0x20;
    PORTD |= (1 << PD3);
    PORTD &= ~(1 << PD3);
    _delay_us(250);
    
    uint8_t command = 0x28;
    lcd_command(command);
    
    command = 0x0C;
    lcd_command(command);
    
    lcd_clear_display();
    
    command = 0x06;
    lcd_command(command);
}

#endif	/* DISPLAY_H */