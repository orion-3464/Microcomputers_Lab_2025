#define F_CPU 16000000UL
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

int flag = 0;
int counter1 = 0;

ISR(INT1_vect) // External INT1 ISR
{   
    flag++;
	//EIFR = (1 << INTF1); // Clear the flag of interrupt INTF1
}

int main(void)
{
	// Interrupt on rising edge of INT1 pin
	EICRA = (1<<ISC11) | (1<<ISC10);
	
	// Enable the INT1 interrupt (PD3))
	EIMSK=(1<<INT1);
	
	sei(); // Enable global interrupts
	
	DDRB = 0xFF; // Set PORTB as output
    DDRD = 0x00;
	
	PORTB=0x00; // Turn off all LEDs of PORTB
    
    while(1)
    {
        PORTB = 0x00;
        flag = 0;
        if(flag > 0){
            while(counter1 < 5000) {
                PORTB = 1;
                _delay_ms(1);
            }
            --flag;
            counter1 = 0;
        }
    }
}