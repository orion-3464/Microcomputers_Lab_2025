#define F_CPU 16000000UL

#include<stdio.h>
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

//                               ..         ..                 ..           ..                
int PWM_values[] = {0x05, 0x1A, 0x2A, 0x42, 0x57, 0x6B, 0x80, 0x98, 0xA8, 0xB9, 0xD1, 0xE6, 0xFA}; 


int main() {
    // configure TMR1
    TCCR1B = (1 << CS12) | (1 << WGM12);
    TCCR1A = (1 << WGM10) | (1 << COM1A1);
    
    // configure ADMUX
    ADMUX = (1 << REFS0) | (1 << MUX0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
    
    // PORTB output
    DDRB = 0xFF;
    
    // POORTC input
    DDRC = 0x00;
    
    // PORTD configure
    DDRD = 0x3F;        //PD6 input for inc Duty Cycle
                        //PD7 input for dec Duty Cycle
    
    int index = 6;          // index for PWM_values array
    int DC_VALUE = PWM_values[6];
    OCR1A = DC_VALUE;
    
   
    int buttons = 0;

    int counter = 0;
    int sum = 0;
    int average = 0;
    
    while(1){
        buttons = PIND;
        buttons &= 0b11000000;
        if(buttons == 0b10000000) {        //PD6
            _delay_ms(150);
            while(buttons == 0b10000000) {
                buttons = PIND;
                buttons &= 0b11000000;
            }
            
            index = (index < 12) ? index+1 : 12;
            DC_VALUE = PWM_values[index];
            OCR1A = DC_VALUE;
        }
        else if(buttons == 0b01000000) {   //PD7
            _delay_ms(150);
            while(buttons == 0b01000000) {
                buttons = PIND;
                buttons &= 0b11000000;
            }
            
            index = (index > 0) ? index-1 : 0;
            DC_VALUE = PWM_values[index];
            OCR1A = DC_VALUE;
        }
        
        if(counter <16){
            _delay_ms(100);
            ADCSRA |= (1 << ADSC);
            sum += ADC;
            ++counter;
        }
        else {
            average = sum>>4;
            sum = 0;
            counter = 0;

            if(average <= 200) PORTD = 0x01;
            else if(average <= 400) PORTD = 0x02;
            else if(average <= 600) PORTD = 0x04;
            else if(average <= 800) PORTD = 0x08;
            else PORTD = 0x10;
        }
        
    }
}
