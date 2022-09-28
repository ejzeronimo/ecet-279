/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 9/19/2022 8:47:49 AM
 * Author: Ethan Zeronik
 *
 * Operations: barebones io testing
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTx.x             LED13 active high
 */

#include <avr/io.h>

#include "Debugger.h"

/* NOTE: Custom Macros */
// TODO: None

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    initDebug();

    IO_init();

    while(1)
    {
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // bottom nibble is motor and top is input switches
    DDRA = 0x0f;
    // turn on switch pullup resistors
    PORTA = 0xf0;

    // the start button
    DDRK = 0x00;
    PORTK = 0x01;

    // the led output port
    DDRC = 0xff;
    PORTC = 0x00;
}
