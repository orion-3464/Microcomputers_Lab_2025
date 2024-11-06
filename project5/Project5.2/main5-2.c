#include "twi.h"

int main() {
    DDRD = 0xFF;
    
    // Init TWI
    twi_init();
    PCA9555_0_write(REG_CONFIGURATION_0, 0x00); //Set EXT_PORT0 as output
    PCA9555_0_write(REG_CONFIGURATION_1, 0xFE); //Set IO1_7:IO1_4 as inputs and IO1_3:IO1_0 as outputs
    
    
    
    while(1) {
        uint8_t input = PCA9555_0_read(REG_INPUT_1);
        PCA9555_0_write(REG_OUTPUT_1, 0x00);
        
        switch(input) {
            case 0b11101110:
                PCA9555_0_write(REG_OUTPUT_0, 0x01);
                break;
                
            case 0b11011110:
                PCA9555_0_write(REG_OUTPUT_0, 0x02);
                break;
                
            case 0b10111110:
                PCA9555_0_write(REG_OUTPUT_0, 0x04);
                break;
                
            case 0b01111110:
                PCA9555_0_write(REG_OUTPUT_0, 0x08);
                break;
                
            default:
                PCA9555_0_write(REG_OUTPUT_0, 0x00);
        }     
    }
}
