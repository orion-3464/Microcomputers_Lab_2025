#define F_CPU 16000000UL
#include "display.h"


#define V_REF 5

float adc_read() {
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC));
    return ADC;
}

int main() {
    // PORTC input
    DDRC = 0x00;
    
    // PORTD output
    DDRD = 0xFF;
    
    // Init ADC
    ADMUX = (1 << REFS0) | (1 << MUX0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    // Initialize display
    lcd_init();
    _delay_ms(5);
    
    
    while(1) {
        lcd_clear_display();
        
        float ADCval = adc_read(); 
        float Vin = ADCval / 1024 * V_REF;
        
        uint8_t Vin_int = (uint8_t) Vin;
        Vin -= Vin_int;
        uint8_t Vin_dec1 =  Vin * 10;
        Vin *= 10;
        Vin -= Vin_dec1;
        uint8_t Vin_dec2 =  Vin * 10;
        
        Vin_int += 48;    // to ASCII
        Vin_dec1 += 48;    // to ASCII
        Vin_dec2 += 48;    // to ASCII
        lcd_data(Vin_int);
        lcd_data('.');
        lcd_data(Vin_dec1);
        lcd_data(Vin_dec2);
        _delay_ms(100);
    }
}
