#ifndef _UART_
#define _UART_

#include <avr/io.h>

/* Routine: usart_init
Description:
This routine initializes the
usart as shown below.
------- INITIALIZATIONS -------
Baud rate: 9600 (Fck= 8MH)
Asynchronous mode
Transmitter on
Reciever on
Communication parameters: 8 Data ,1 Stop, no Parity
--------------------------------
parameters: ubrr to control the BAUD.
return value: None.*/

void usart_init(unsigned int ubrr){
    UCSR0A=0;
    UCSR0B=(1<<RXEN0)|(1<<TXEN0);
    UBRR0H=(unsigned char)(ubrr>>8);
    UBRR0L=(unsigned char)ubrr;
    UCSR0C=(3 << UCSZ00);
    return;
}

/* Routine: usart_transmit
Description:
This routine sends a byte of data
using usart.
parameters:
data: the byte to be transmitted
return value: None. */

void usart_transmit(uint8_t data) {
    while(!(UCSR0A&(1<<UDRE0)));
    UDR0=data;
}

/* Routine: usart_receive
Description:
This routine receives a byte of data
from usart.
parameters: None.
return value: the received byte */

uint8_t usart_receive() {
    while(!(UCSR0A&(1<<RXC0)));
    return UDR0;
}

// ------------------------------------- //
void send_string(const char *m) {
    int i = 0;
    while(m[i] != '\0') {
        usart_transmit(m[i]);
        ++i;
    }
}

void receive_string(char *buffer) {
    char c;
    int i = 0;
    while((c = usart_receive()) != '\n') {
        buffer[i] = c;
        ++i;
        if(i >= 64) break;
    }
    buffer[i] = '\0';
}

#endif /*_UART_*/
