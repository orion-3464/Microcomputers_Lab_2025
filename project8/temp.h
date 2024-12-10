#ifndef _TEMP_
#define _TEMP_

#include "onewire.h"

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
    return temperature;
}

float translate_meas(uint16_t t) {
    return 0.0625 * t;
}

void print_temperature(float t) {
    uint8_t tint = (uint8_t) t;
    uint8_t t3 = tint / 100;
    uint8_t tint1 = tint - t3 * 100;
    uint8_t t2 = tint1 / 10;
    uint8_t tint2 = tint1 - t2 * 10;
    uint8_t t1 = tint2;
    
    float tdec = t - tint;
    uint8_t tdec1 = tdec * 10;
    tdec *= 10;
    tdec -= tdec1;
    uint8_t tdec2 = tdec * 10;
    tdec *= 10;
    tdec -= tdec2;
    uint8_t tdec3 =  tdec*10;
    
    lcd_data(t3 + 48);
    lcd_data(t2 + 48);
    lcd_data(t1 + 48);
    lcd_data('.');
    lcd_data(tdec1 + 48);
    lcd_data(tdec2 + 48);
    lcd_data(tdec3 + 48);
    lcd_data('C');
}

void print_no_device() {
    lcd_data('N');
    lcd_data('O');
    lcd_data(' ');
    lcd_data('D');
    lcd_data('E');
    lcd_data('V');
    lcd_data('I');
    lcd_data('C');
    lcd_data('E');
}

#endif /*_TEMP_*/
