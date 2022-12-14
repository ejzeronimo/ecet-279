/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 8/24/2022 2:09 PM
 * Author: Ethan Zeronik
 *
 * Operations: turn on board LED every half second
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTD.7             LED13 active high
 */

#include <avr/io.h>

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();

    while(1)
    {
        // DEBUG: comment this line out when using simulator
        _delay_ms(500);

        // set it to the inverse of the current value
        PORTB = ~(0x80 & PORTB);
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // set led 13 as output
    DDRB  = 0x80;
    // turn off led on init
    PORTB = 0x80;
}

