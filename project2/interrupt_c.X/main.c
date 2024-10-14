#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR (INT0_vect) {           //External INT0 ISR
    PORTC = 0xFF;
    _delay_ms(2000);
    PORTC = 0x00;
}

ISR (INT1_vect) {           //External INT1 ISR
    PORTC = 0xFF;
    _delay_ms(2000);
    PORTC = 0x00;
}

int main(void) {            
    // Interrupt on rising edge of INT0 and INT1 pin
    EICRA=(1 << ISC01) | (1 << ISC00) | (1 << ISC11) | (1 << ISC10);
    // Enable the INT0 interrupt (PD2), INT1 interrupt (PD3)
    EIMSK=(1 << INT0) | (1 << INT1);
    sei();
    
    DDRB=0XFF;
    DDRC=0XFF;
    PORTC=0x00;
    
    while(1) {
        PORTB = 0x00;
        _delay_ms(500);
        PORTB = 0xFF;
        _delay_ms(500);
    }
}
