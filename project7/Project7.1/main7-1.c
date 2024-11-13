#include "onewire.h"

uint16_t temp_read();

int main() {
    DDRD = 0xFF;    // PORTD as output
    DDRB = 0xFF;
    PORTB = 0;
    _delay_ms(3000);
    
    while(1) {
        uint16_t meas = temp_read();
        while(meas == 0x8000) PORTB = 0xFF;
        if((meas & 0xF800) == 0xF800) {
            PORTB = 0b11110000;
            _delay_ms(3000);
        }
        else {
            PORTB = 0b00001111;
            _delay_ms(3000);
        }
        PORTB = 0;
        _delay_ms(3000);
    }
    
}

uint16_t temp_read() {
    if(!one_wire_reset()) return 0x8000;    // check if device is connected
    
    one_wire_transmit_byte(0xCC);   // disable multidevice
    one_wire_transmit_byte(0x44);   // send 0x44 command and start measuring
    
    
    while(one_wire_receive_bit() != 0x01);      // wait until device 
                                                //stops the conversion
    
    if(!one_wire_reset()) return 0x8000;    // init device again
    
    one_wire_transmit_byte(0xCC);   // disable multidevice
    one_wire_transmit_byte(0xBE);   // send 0xBE command and 
                                    // start measuring 16 - bit
    
    // Read temperature
    uint16_t temp_low, temp_high;
    uint16_t temperature;
    
    temp_low = one_wire_receive_byte();
    temp_high = one_wire_receive_byte();
    
    temperature = (temp_high << 8) | temp_low; // maybe +
    // return value considering 2's compliment representation
    //return ((temp_high & 0xF8) == 0xF8) ?  ~temperature + 1 : temperature;
    return temperature;
}