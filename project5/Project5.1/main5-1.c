#include "twi.h"

int main() {
    // PORTB input
    DDRB = 0x00;
   
    // Init TWI
    twi_init();
    PCA9555_0_write(REG_CONFIGURATION_0, 0x00); //Set EXT_PORT0 as output
    
    while(1) {
        // Read A, B, C, D to LSB
        uint8_t input = PINB;
        uint8_t A = input & 0x01;
        uint8_t B = (input & 0x02) >> 1;
        uint8_t C = (input & 0x04) >> 2;
        uint8_t D = (input & 0x08) >> 3;
        
        uint8_t f0 = (~(((~A)&B&C) | ((~B)&D))) & 0x01;
        uint8_t f1 = (((A | B | C) & (B & (~D)))) & 0x01;
        
        PCA9555_0_write(REG_OUTPUT_0, f0 | (f1 << 1));      
    }
}