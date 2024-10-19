#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main() {
    unsigned char duty;
    
    //set TMR1A in fast PWM 8 bit mode with non-inverted output
    //prescale=8
    TCCR1A = (1<<WGM10) | (1<<COM1A1);
    TCCR1B = (1<<WGM12) | (1<<CS11);
    
    DDRB |= 0b00111111;      //set PB5-PB0 pins as output
            
    while(1) {
        for(duty = 0; duty < 255; duty++) {
            OCR1AL = duty;  //increase the LED2 light intensity
            _delay_ms(10);
        }
        for(duty = 255; duty > 1; duty--) {
            OCR1AL = duty;  // decrease the LED2 light intensity
            _delay_ms(10);
        }
    }
            
}
