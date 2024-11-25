#include "keypad.h"

int main() {    
    // PORTB OUTPUT
    DDRB = 0xFF;
    
    twi_init();
    
    // IO1[3:0] OUTPUTS
    // IO1[7:4] INPUTS
    PCA9555_0_write(REG_CONFIGURATION_1, 0xF0);
    
    // Setting IO1[3:0] HIGH
    PCA9555_0_write(REG_OUTPUT_1, 0xFF);
    
    /* 
      When the rows are set to high the INPUT pins read
      logic 1 whether the button is pressed or not
    */
    
    while(1) {
        scan_keypad_rising_edge();
        button2led(pressed_keys);
    }
    
}
