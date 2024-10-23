#define F_CPU 16000000UL

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

int mode = 1;           // default mode = 1

int PWM_values[] = {0x05, 0x1A, 0x2E, 0x42, 0x57, 0x6B, 0x80, 0x94, 0xA8, 0xBD, 0xD1, 0xE6, 0xFA}; 

void select_mode();

int main() {    
    // PORTB output
    DDRB = 0xFF;
    
    // PORTC input
    DDRC = 0x00;
    
    // PORTD input
    DDRD = 0x00;        //PD6 input for inc Duty Cycle
                        //PD7 input for dec Duty Cycle
    
    
    while(1) {
        select_mode();
        // mode 1 functionality
        if(mode == 1) { 
            // configure TMR1
            TCCR1B = (1 << CS12) | (1 << WGM12);
            TCCR1A = (1 << WGM10) | (1 << COM1A1);
            
            int index = 6;          // index for PWM_values array
            int DC_VALUE = PWM_values[6];
            OCR1A = DC_VALUE;
            
            while(mode == 1) {
                if(PIND == 0b11111101) {            //buttons function with negative logic
                    _delay_ms(200);
                    while(PIND != 0b11111111) {
                        ;
                    }
                    index = (index < 12) ? index+1 : 12;
                    DC_VALUE = PWM_values[index];
                    OCR1A = DC_VALUE;
                }
                else if(PIND == 0b11111011) {
                    _delay_ms(200);
                    while(PIND != 0b11111111) {
                        ;
                    }
                    index = (index > 0) ? index-1 : 0;
                    DC_VALUE = PWM_values[index];
                    OCR1A = DC_VALUE;
                } 
                
                select_mode();
            }
        }
        
        else if(mode == 2) {
            // Configure ADC0 (connected to POT1) on AVCC reference, ADC0 pin
            ADMUX = (1 << REFS0);                  // AVCC with external capacitor at AREF pin
            ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);  // Enable ADC, prescaler 64
            
            while(mode == 2) {
                ADCSRA |= (1 << ADSC);  // Start conversion
                uint16_t ADCval = ADC;
                uint8_t PWMval = ADCval>>2;

                OCR1A = PWMval;
                select_mode();
            }
        }
    }
}

void select_mode() {
    // select mode
    if(PIND == 0b10111111) {                 //buttons function with negative logic
        _delay_ms(200);
        while(PIND != 0b11111111) {
            ;
        }
        mode = 1;
    }
    else if(PIND == 0b01111111) {            //buttons function with negative logic
        _delay_ms(200);
        while(PIND != 0b11111111) {
            ;
        }
        mode = 2;
    }
}
