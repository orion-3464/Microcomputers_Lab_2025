#include "twi.h"
#include "keypad.h"
#include "display.h"

int main() {
    // setting PORTD output because of display functions
    DDRD = 0xFF;
    
// -------------- twi Initializations ---------------- //
    twi_init();
    
    // IO1[3:0] OUTPUTS
    // IO1[7:4] INPUTS
    PCA9555_0_write(REG_CONFIGURATION_1, 0xF0);
    
    // Setting IO1[3:0] HIGH
    PCA9555_0_write(REG_OUTPUT_1, 0xFF);
    
// -------------- lcd Initializations --------------- //
    lcd_init();
    _delay_ms(5);
    
    int print_char;
    int pressed_keys = 0;
    
    while(1) {
        while (pressed_keys == 0) {
            pressed_keys = scan_keypad_rising_edge();    
        }
        print_char = key_pad_to_ascii(pressed_keys);
        lcd_data(print_char);
        lcd_command(0x80);
        pressed_keys = 0;
    }
}
