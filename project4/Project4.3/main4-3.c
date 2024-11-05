#define F_CPU 16000000UL
#include "display.h"

#define V_REF 5

float adc_read() {
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC));
    return ADC;
}

void print_clear() {
    lcd_clear_display();
    
    lcd_data('C');
    lcd_data('L');
    lcd_data('E');
    lcd_data('A');
    lcd_data('R');
}

void print_gas() {
    lcd_clear_display();
    
    lcd_data('G');
    lcd_data('A');
    lcd_data('S');
    lcd_data(' ');
    lcd_data('D');
    lcd_data('E');
    lcd_data('T');
    lcd_data('E');
    lcd_data('C');
    lcd_data('T');
    lcd_data('E');
    lcd_data('D');
}

float convert(float Vin) {
    float Vgas0 = 0.1;
    float M = 0.0129;
    float Cx =  (1 / M) * (Vin - Vgas0);
    
    return Cx;
}

int main() {
    // PORTD output
    DDRD = 0xFF;
    
    // PORTC input
    DDRC = 0x00;
    
    // PORTB output
    DDRB = 0b11111111;  // PD5 - PD0 output
    
    // Init ADC
    ADMUX = (1 << REFS0) | (1 << MUX1);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    // Display init
    lcd_init();
    _delay_ms(5);
    
    while(1) {
        float ADCval = adc_read();
        float vin = (ADCval / 1024) * V_REF;
        float CO = convert(vin);
        
        if(CO >= 0 && CO <= 70) {
            PORTB = 0x01;
            print_clear();
        }
        else if(CO > 70 && CO <= 140) {
            PORTB = 0x03;
            print_gas();
            _delay_ms(500);
            PORTB = 0x00;
            _delay_ms(500);
        }
        else if(CO > 140 && CO <= 210) {
            PORTB = 0x07;
            print_gas();
            _delay_ms(500);
            PORTB = 0x00;
            _delay_ms(500);
        }
        else if(CO > 210 && CO <= 280) {
            PORTB = 0x0F;
            print_gas();
            _delay_ms(500);
            PORTB = 0x00;
            _delay_ms(500);
        }
        else if(CO > 280 && CO <= 350) {
            PORTB = 0x1F;
            print_gas();
            _delay_ms(500);
            PORTB = 0x00;
            _delay_ms(500);
        }
        else if(CO > 350 && CO <= 420){
            PORTB = 0x3F;
            print_gas();
            _delay_ms(500);
            PORTB = 0x00;
            _delay_ms(500);
        }
        else {
            PORTB = 0x01;
        }
        _delay_ms(100);
        
    }
    
}
