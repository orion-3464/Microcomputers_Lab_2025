#include "onewire.h"
#include "display_ext.h"
#include "twi.h"

uint16_t temp_read();
float translate_meas(uint16_t t);
void print_temperature(float t);
void print_no_device();

int main() {
    //Init twi
    twi_init();
    
    PCA9555_0_write(REG_CONFIGURATION_0, 0x00);     //Set EXT_PORT0 as output
    
    // Display init
    lcd_init();
    _delay_ms(50);
    
    int reconnected = 0;
    lcd_clear_display();
    
    DDRD = 0xFF;
    
    while(1) {
        lcd_command(0x80);
        uint16_t meas = temp_read();
        while(meas == 0x8000) {
            print_no_device();
            meas = temp_read();
            lcd_command(0x80);
            reconnected = 1;
        }
        
        if(reconnected) {
            lcd_clear_display();
            reconnected = 0;
        }
        // if measurement is negative
        if((meas & 0xF800) == 0xF800) {
            lcd_data('-');
            meas = ~meas + 1;
            float temp = translate_meas(meas);
            print_temperature(temp);
        }
        else {
            lcd_data('+');
            float temp = translate_meas(meas);
            print_temperature(temp);
        } 
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
