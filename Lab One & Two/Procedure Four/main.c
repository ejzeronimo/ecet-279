/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 8/31/2022 1:39 PM
 * Author: Ethan Zeronik
 *
 * Operations: sweeps an led bar from right to left
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTA               LED bar (all 8 pins used)
 */

#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);
// sweeps the given port's bits from 0x00 to 0xff then back to 0x00
void LED_sweep(volatile uint8_t * port);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();

    while(1)
    {
        // run the sweep
        LED_sweep(&PORTA);
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // set all 8 leds as outputs
    DDRA  = 0xFF;
    // turn off all leds
    PORTA = 0x00;
}

void LED_sweep(volatile uint8_t * port)
{
    while(!(PORTA & 0x80))
    {
        // DEBUG: comment this line out when using simulator
        _delay_ms(500);

        // move over leds by one
        *port = *port << 1;
        // then add a new bit at LSB
        *port = *port | 0x01;
    }

    // send it back to 0
    while(!(PORTA == 0x00))
    {
        // DEBUG: comment this line out when using simulator
        _delay_ms(500);

        // move over leds by one
        *port = *port >> 1;
    }
}
