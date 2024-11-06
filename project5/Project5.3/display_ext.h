#ifndef DISPLAY_H
#define	DISPLAY_H

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "twi.h"

void write_2_nibbles(uint8_t data) {
    uint8_t prev;
    uint8_t current;
    uint8_t temp;
    
    prev = PCA9555_0_read(REG_INPUT_0) & 0x0F;
    current = (data & 0xF0) | prev;
    PCA9555_0_write(REG_OUTPUT_0, current);
    temp = PCA9555_0_read(REG_INPUT_0) | (1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    temp = PCA9555_0_read(REG_INPUT_0) & ~(1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    
    data <<= 4;
    current = (data & 0xF0) | prev;
    PCA9555_0_write(REG_OUTPUT_0, current);
    temp = PCA9555_0_read(REG_INPUT_0) | (1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    temp = PCA9555_0_read(REG_INPUT_0) & ~(1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
}

void lcd_data(uint8_t byte) {
    uint8_t temp;
    temp = PCA9555_0_read(REG_INPUT_0) | (1 << PD2);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    write_2_nibbles(byte);
    _delay_us(250);
}

void lcd_command(uint8_t byte) {
    uint8_t temp;
    temp = PCA9555_0_read(REG_INPUT_0) & ~(1 << PD2);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    write_2_nibbles(byte);
    _delay_us(250);
}

void lcd_clear_display() {
    uint8_t command = 0x01;
    lcd_command(command);
    _delay_ms(5);
}

void lcd_init() {
    uint8_t temp;
    _delay_ms(200);
    
    temp = 0x30;
    PCA9555_0_write(REG_OUTPUT_0, temp);
    temp = PCA9555_0_read(REG_INPUT_0) | (1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    temp = PCA9555_0_read(REG_INPUT_0) & ~(1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    _delay_us(250);
    
    temp = 0x30;
    PCA9555_0_write(REG_OUTPUT_0, temp);
    temp = PCA9555_0_read(REG_INPUT_0) | (1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    temp = PCA9555_0_read(REG_INPUT_0) & ~(1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    _delay_us(250);
    
    temp = 0x30;
    PCA9555_0_write(REG_OUTPUT_0, temp);
    temp = PCA9555_0_read(REG_INPUT_0) | (1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    temp = PCA9555_0_read(REG_INPUT_0) & ~(1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    _delay_us(250);
    
    temp = 0x20;
    PCA9555_0_write(REG_OUTPUT_0, temp);
    temp = PCA9555_0_read(REG_INPUT_0) | (1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
    temp = PCA9555_0_read(REG_INPUT_0) & ~(1 << PD3);
    PCA9555_0_write(REG_OUTPUT_0, temp);
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