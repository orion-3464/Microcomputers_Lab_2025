/*
 * File:   main.c
 * Author: team12
 *
 * Created on October 15, 2024
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int interrupted = 0;

ISR (INT1_vect) {               // External INT1 ISR
    PORTB = 0x01;
    _delay_ms(5000);
    PORTB = 0x00;
}

int main(void) {
    // Interrupt on rising edge of INT0 and INT1 pin
    EICRA = (1 << ISC11) | (1 << ISC10); 
    // Enable the INT1 interrupt (PD3)
    EIMSK = (1 << INT1);
    sei();          // Enable global interrupts
    
    DDRB = 0xFF;    // PORTB is output
    DDRD = 0x00;    // PORTD is input
    
    while (1) {
        interrupted = 0;
        //PORTB = 0x00;
    }
}
