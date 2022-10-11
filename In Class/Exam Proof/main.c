/*
 * Exam Proof.c
 *
 * Created: 9/25/2022 3:13:24 PM
 * Author : Ethan Zeronik
 */

#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

int main(void)
{
    // the top nibble is the motor output while the bottom is button input
    DDRA  = 0xFF;
    // turn on pullup resisitors on the bottom nibble
    PORTA = 0x00;

    _delay_ms(1000);

    // first question
    PORTA = (PORTA & ~0x18) | 0x18;
    _delay_ms(1000);

    // second question
    PORTA = (PORTA & ~0x18) | 0x00;
    _delay_ms(1000);

    while(1)
    {
        PORTA = (PORTA & ~0x18) | (~PORTA & 0x18);
		
		_delay_ms(1000);
    }
}
