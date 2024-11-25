#ifndef KEYPAD_H
#define KEYPAD_H

#include "twi.h"

#define row4 0b00001110 // we have the 4 msb as '0' 
#define row3 0b00001101 // this helps in the scan_row function
#define row2 0b00001011
#define row1 0b00000111

uint16_t pressed_keys;
// --------------------------------------------------------------------- //

int scan_row(uint8_t row) {
    // tests one row for pressed buttons and returns the number of the 
    // column of the pressed button or 0 if there are no pressed buttons
    
    PCA9555_0_write(REG_OUTPUT_1, row); 
    uint8_t input = PCA9555_0_read(REG_INPUT_1);
    
    if (input == (0b11100000 | row)){
        return 1;
    }
    else if (input == (0b11010000 | row)){
        return 2;
    }
    else if (input == (0b10110000 | row)){ 
        return 3;
    }
    else if (input == (0b01110000 | row)){
        return 4;
    }
    else {
        return 0;
    }
}
// --------------------------------------------------------------------- //

int scan_keypad(void){
    int temp;
    
    temp = scan_row(row1);
    if (temp != 0) 
        return temp;
    
    temp = scan_row(row2);
    if (temp != 0) 
        return temp+4;
    
    temp = scan_row(row3);
    if (temp != 0) 
        return temp+8;
    
    temp = scan_row(row4);
    if (temp != 0) 
        return temp+12;
    
    return 0;
}

// --------------------------------------------------------------------- //

uint16_t scan_keypad_rising_edge(void){//keeps a recording of pressed button
    
    uint16_t pressed_keys;
       
    pressed_keys = scan_keypad();
    // Sparkling effect
    _delay_ms(10);
    pressed_keys = scan_keypad();
    
    return pressed_keys;
}

int key_pad_to_ascii(uint16_t pressed_keys) {
    
    if (pressed_keys%4 == 0) {
        return 'A' + (pressed_keys/4);
    }
    else if (pressed_keys == 13){
        return '*';
    }
    else if (pressed_keys == 15){
        return '#';
    }
    else {
        return 48 + pressed_keys;
    }
}

void button2led (uint16_t pressed_keys) {
//    int number = int(pressed_keys);
    switch (pressed_keys){
        case 4:
            PORTB = 0x01;
            break;
        case 10:
            PORTB = 0x02;
            break;
        case 7:
            PORTB = 0x04;
            break;
        case 13:
            PORTB = 0x08;
            break;
        default:
            PORTB = 0x00;
    }
}

#endif	/* KEYPAD_H */
