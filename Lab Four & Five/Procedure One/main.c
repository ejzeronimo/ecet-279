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
 *   PORTA.4             hot water switch
 *   PORTA.5             warm water switch
 *   PORTA.6             cold water switch
 *   PORTA.7             door open switch
 *   PORTK.0             start pushbutton
 *   PORTA.0             motor out 1
 *   PORTA.1             motor out 2
 *   PORTA.2             motor out 3
 *   PORTA.3             motor out 4
 *   PORTC.0             wash done led
 *   PORTC.1             agitate led
 *   PORTC.2             spin led
 *   PORTC.4             drain valve
 *   PORTC.5             hot water valve
 *   PORTC.6             cold water valve             
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
