/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 9/14/2022 1:51 PM
 * Author: Ethan Zeronik
 *
 * Operations: test the io config and the debugger for the lab 
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTA.0-3           button inputs (pullup resistors on)
 *   PORTA.4-7           led output
 */

/* NOTE: Includes */
#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

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
        PORTA = (0x55 & 0xF0) | (PORTA & 0x0F);
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // the top nibble is the motor output while the bottom is button input
    DDRA  = 0xF0;
    // turn on pullup resisitors on the bottom nibble
    PORTA = 0x0F;
}