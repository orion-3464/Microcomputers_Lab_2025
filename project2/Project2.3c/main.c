#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

int flag = 0;
int counter1 = 0;
int counter2 = 0;

ISR(INT1_vect) { // External INT1 ISR
    counter1 = 0;
    counter2 = 0;
    flag++;
    if(flag >= 2) flag = 2;
    
    sei();
    
    
    if(flag > 0){
        while(counter1 < 5000){
            PORTB = 0x01;
            _delay_ms(1);
            counter1++;
            if(flag > 1) {
                while(counter2 < 500) {
                    PORTB = 0xFF;
                    _delay_ms(1);
                    counter2++;
                }
                flag--;
                if(flag < 0) flag = 0;
            }
        }
        flag--;
        if(flag < 0) flag = 0;
    }
}

int main(void) {
    // Interrupt on rising edge of INT1 pin
    EICRA = (1<<ISC11) | (1<<ISC10);

    // Enable the INT1 interrupt (PD3)
    EIMSK = (1<<INT1);

    sei(); // Enable global interrupts

    DDRD = 0x00; // Set PORTD as input

    DDRB = 0xFF; // Set PORTB as output
    
    while(1) {
        PORTB = 0x00;
    }
}
