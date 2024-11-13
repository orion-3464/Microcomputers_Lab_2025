#ifndef ONEWIRE_H
#define	ONEWIRE_H

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define cbi(register, bit) (register &= ~(1 << bit))
#define sbi(register, bit) (register |= (1 << bit))

uint8_t one_wire_reset() {
    sbi(DDRD, PD4);     // Set PD4 as output
    
    cbi(PORTD, PD4);
    _delay_us(480);     // 480 usec reset pulse
    
    cbi(DDRD, PD4);     // Set PD4 as input
    cbi(PORTD, PD4);    // disable pull-up
    
    _delay_us(100);     // wait 100us for connected devices to transmit
                        // the presence pulse
    
    uint8_t temp = PIND;        // Read PORTD
    
    _delay_us(380);     // wait for 380usec
    
    temp &= 0x10;
    
    if(temp == 0) return 1;
    
    return 0;
}

uint8_t one_wire_receive_bit() {
    sbi(DDRD, PD4);     // set PD4 as output
    
    cbi(PORTD, PD4); 
    _delay_us(2);       // time slot 2 usec
    
    cbi(DDRD, PD4);     // set PD4 as input
    cbi(PORTD, PD4);    // disable pull-up
    
    _delay_us(10);      // wait 10 usec
    
    uint8_t bit = 0;
    if((PIND & 0x10) == 0x10) bit = 1;
    
    _delay_us(49);      // delay 49usec to meet the standards
    
    return bit;
}

void one_wire_transmit_bit(uint8_t bit) {
    sbi(DDRD, PD4);      // set PD4 as output
    cbi(PORTD, PD4);     // time slot 2usec
    
    _delay_us(2);
    
    (bit == 0x00) ? cbi(PORTD, PD4) : sbi(PORTD, PD4);      // PD4 = r24[0]
    
    _delay_us(58);      // wait 58 usec for connected device to sample the line
    
    cbi(DDRD, PD4);      // set PD4 as input
    cbi(PORTD, PD4);    // disable pull up
        
    _delay_us(1);       // recovery time 1 usec
    
    return;
}

uint8_t one_wire_receive_byte() {    
    uint8_t bit = 0x00;
    uint8_t byte = 0x00;
    
    for(int i = 8; i > 0; --i) {
        bit = one_wire_receive_bit();
        byte >>= 1;
        if(bit == 0x01) bit = 0x80;
        byte |= bit;
    }
   
    return byte;
}

void one_wire_transmit_byte(uint8_t byte) {
    uint8_t bit;
    
    for(int i = 8; i > 0; --i) {
        bit = (byte & 0x01);
        one_wire_transmit_bit(bit);
        byte >>= 1;
    }
    return;
}

#endif	/* ONEWIRE_H */