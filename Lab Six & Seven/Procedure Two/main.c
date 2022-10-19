/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 10/18/2022 8:38:44 PM
 * Author: Ethan Zeronik
 *
 * Operations: Blink and LED every 500ms
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTB.7             LED13 active high
 */

/* NOTE: Includes */
#include <avr/io.h>

#include "Delay.h"
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
    DLY_init();

    IO_init();

    initDebug();

    while(1)
    {
        for(size_t i = 0; i < 100; i++)
        {
            DLY_ms(500);
        }

        PORTB = ~(0x80 & PORTB);
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // set port B.7 as an output
    DDRB  = 0x80;
    PORTB = 0x00;
}
