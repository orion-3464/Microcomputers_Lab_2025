#include "twi.h"
#include "keypad.h"

int main() {
    // setting PORTB and PORTD as outputs
    DDRB = 0xFF;
    
    twi_init();
    
    // IO1[3:0] OUTPUTS
    // IO1[7:4] INPUTS
    PCA9555_0_write(REG_CONFIGURATION_1, 0xF0);
    
    // Setting IO1[3:0] HIGH
    PCA9555_0_write(REG_OUTPUT_1, 0xFF);
    
    
    uint16_t key1 = 0;
    uint16_t key2 = 0;
    
    while(1) {
        while(key1 == 0) {
            key1 = scan_keypad_rising_edge();
        }
        
        while(scan_keypad_rising_edge() != 0) ;
        
        while(key2 == 0) {
            key2 = scan_keypad_rising_edge();
        }
        
        while(scan_keypad_rising_edge() != 0) ;
        
        if (key1 == 1 && key2 == 2) {
            PORTB = 0xFF;
            _delay_ms(3000);
        }
        else {
            for(int i=0; i<5; ++i){
                PORTB = 0xFF;
                _delay_ms(500);
                PORTB = 0x00;
                _delay_ms(500);
            }
        }
        key1 = 0;
        key2 = 0;
        PORTB = 0;
        _delay_ms(5000);
    }
}
