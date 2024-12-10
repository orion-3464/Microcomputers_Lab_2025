#define F_CPU 16000000UL

#include "uart.h"
#include "twi.h"
#include "display_ext.h"
#include "temp.h"
#include "utils.h"
#include "keypad.h"
#include <string.h>

#define _MESSAGE_BUFFER_SIZE_ 64
#define _PAYLOAD_SIZE_ 512

int main() {
    // UART init
    usart_init(103);
    
    // TWI init
    twi_init();

    //Set EXT_PORT0 as output
    PCA9555_0_write(REG_CONFIGURATION_0, 0x00);
    
    // IO1[7:4] INPUTS
    PCA9555_0_write(REG_CONFIGURATION_1, 0xF0);
    
    // Setting IO1[3:0] HIGH
    PCA9555_0_write(REG_OUTPUT_1, 0xFF);
    
    // Display init
    lcd_init();
    _delay_ms(50);
    lcd_clear_display();
    
    //PORTC input
    DDRC = 0x00;
    DDRD = 0xFF;
    
    // configure ADMUX
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
    
    
    // Restart ESP module
    send_string("ESP:restart\n");
    
    char buf[_MESSAGE_BUFFER_SIZE_] = {0};    
    int reconnected = 0;    // for the temperature sensor
    char payload[_PAYLOAD_SIZE_] = {0};
    int status = 4;
    
    while(1) {
        lcd_clear_display();
        memset(buf, 0, _MESSAGE_BUFFER_SIZE_);
        /*-------------------------------- QUESTION 8.1 -----------------------------------*/
        send_string("ESP:connect\n");
        receive_string(buf);
        if( strcmp(buf, "\"Success\"") ) {
            for(int i=0; i<1000;++i) {
                lcd_command(0x80);
                print_connect_attemp();
                memset(buf, 0, _MESSAGE_BUFFER_SIZE_);
                send_string("ESP:connect\n");
                receive_string(buf);
                _delay_ms(10);
                if(!strcmp(buf,"\"Success\""))
                    break;
            }
        }
            
        if(!strcmp(buf, "\"Success\"")){
            lcd_clear_display();
            print_lcd_success(1);
        }
        else {
            lcd_clear_display();
            print_lcd_fail(1);
        }
        
        _delay_ms(3000);
        memset(buf, 0, _MESSAGE_BUFFER_SIZE_);
            
        send_string("ESP:url:\"http://192.168.1.250:5000/data\"\n");
        receive_string(buf);
        if( strcmp(buf, "\"Success\"") ) {
            for(int i=0; i<1000;++i) {
                lcd_command(0x80);
                print_connect_attemp();
                memset(buf, 0, _MESSAGE_BUFFER_SIZE_);
                send_string("ESP:url:\"http://192.168.1.250:5000/data\"\n");
                receive_string(buf);
                _delay_ms(10);
                if(!strcmp(buf,"\"Success\""))
                    break;
            }
        }
        
        if(!strcmp(buf, "\"Success\"")) {
            lcd_clear_display();
            print_lcd_success(2);
        }
        else {
            lcd_clear_display();
            print_lcd_fail(2);
        }
         
        /*-------------------------------- QUESTION 8.2 -----------------------------------*/ 
        _delay_ms(3000);
        lcd_clear_display();
        
         // ---- READ TEMPERATURE SENSOR ---- //
        uint16_t temp_meas = temp_read();
        while(temp_meas == 0x8000) {
            print_no_device();
            temp_meas = temp_read();
            lcd_command(0x80);
            reconnected = 1;
        }
        
        if(reconnected) {
            lcd_clear_display();
            reconnected = 0;
        }
        
        float temp = translate_meas(temp_meas);
        temp += 14;
        
        // ---- READ POTENSIOMETER ---- //
        float pres;
        float ADC_value;
        
        ADC_value = adc_read();
        pres = ADC_value/1023 * 20; // mapping to 0-20 cm H2O
        
        // ---- CHECK STATUS ---- // 
        status = 4;
        
        print_message();
        int key = 0;
        for(int i=0; i<500; ++i){
            key = scan_keypad_rising_edge(); //scan_keypad_rising_edge() has 10msec delay
            if(key == 2) {
                status = 1;
                break;
            }
        }
        while(scan_keypad_rising_edge() == 2);
    
        if(key == 2){
            lcd_clear_display();
            lcd_nurse_call();
            lcd_command(0xC0);
            _delay_ms(500);
            for(int i=0; i<500; ++i){
                int sec = i / 100;
                lcd_data((5 - sec) + 48);
                lcd_command(0xC0);

                if(scan_keypad_rising_edge() == 15) {
                    status = 4;
                    while(scan_keypad_rising_edge() == 15);
                    break;
                }
            }
        }
        lcd_clear_display();
        if(status == 4 && (pres < 4 || pres > 12))
            status = 2;
        else if(status == 4 && (temp < 34 || temp > 37))
            status = 3;
        
        // PRINT EVERYTHING TO LCD
        print_temperature(temp);
        lcd_data(' ');
        lcd_data('~');
        lcd_data(' ');
        
        print_pressure(pres);
        lcd_command(0xC0);  // Force cursor to the beginning (2nd line)
        
        print_status(status);
        
        _delay_ms(3000);
        memset(buf, 0, _MESSAGE_BUFFER_SIZE_);
        lcd_clear_display();
        
        
        // ---- FORM PAYLOAD ---- //
        form_payload(payload, temp, pres, status); 
        send_string(payload);   //send payload
        receive_string(buf);
        if( strcmp(buf, "\"Success\"") ) {
            for(int i=0; i<1000;++i) {
                lcd_command(0x80);
                print_connect_attemp();
                memset(buf, 0, _MESSAGE_BUFFER_SIZE_);
                send_string(payload);
                receive_string(buf);
                _delay_ms(10);
                if(!strcmp(buf,"\"Success\""))
                    break;
            }
        }
        if(!strcmp(buf, "\"Success\"")) {
            lcd_clear_display();
            print_lcd_success(3);
        }
        else {
            lcd_clear_display();
            print_lcd_fail(3);
        }
        
        _delay_ms(3000);
        memset(buf, 0, _MESSAGE_BUFFER_SIZE_);
        lcd_clear_display();
        
        send_string("ESP:transmit\n");
        receive_string(buf);
       
        int j=0;
        while(buf[j] != '\0' && buf[j] != '\n') {
            lcd_data(buf[j]);
            j++;
        }
        _delay_ms(3000);
    }
    
    return 0;
}

