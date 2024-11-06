#include "display_ext.h"

void print_name();

int main() {
    //Init twi
    twi_init();
    
    PCA9555_0_write(REG_CONFIGURATION_0, 0x00);     //Set EXT_PORT0 as output
    
    // Display init
    lcd_init();
    _delay_ms(50);
    
    while(1) {
        print_name();
    }
}

void print_name() {
    lcd_clear_display();
    _delay_ms(10);
    
    lcd_data('N');
    _delay_ms(100);
    lcd_data('I');
    _delay_ms(100);
    lcd_data('K');
    _delay_ms(100);
    lcd_data('O');
    _delay_ms(100);
    lcd_data('S');
    _delay_ms(10);
    lcd_command(0xC0);
    lcd_data('K');
    _delay_ms(100);
    lcd_data('O');
    _delay_ms(100);
    lcd_data('R');
    _delay_ms(100);
    lcd_data('D');
    _delay_ms(100);
    lcd_data('A');
    _delay_ms(100);
    lcd_data('S');
    _delay_ms(100);
 
    _delay_ms(1000);
    
    lcd_clear_display();
    _delay_ms(100);
    lcd_data('D');
    _delay_ms(100);
    lcd_data('I');
    _delay_ms(100);
    lcd_data('M');
    _delay_ms(100);
    lcd_data('I');
    _delay_ms(100);
    lcd_data('T');
    _delay_ms(100);
    lcd_data('R');
    _delay_ms(100);
    lcd_data('I');
    _delay_ms(100);
    lcd_data('S');
    _delay_ms(100);
    lcd_command(0xC0);
    _delay_ms(100);
    lcd_data('P');
    _delay_ms(100);
    lcd_data('A');
    _delay_ms(100);
    lcd_data('N');
    _delay_ms(100);
    lcd_data('T');
    _delay_ms(100);
    lcd_data('A');
    _delay_ms(100);
    lcd_data('Z');
    _delay_ms(100);
    lcd_data('I');
    _delay_ms(100);
    lcd_data('S');
    _delay_ms(100);
    _delay_ms(1000);
}
