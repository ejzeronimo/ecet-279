/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 10/11/2022 5:36:00 PM
 * Author: Ethan Zeronik
 *
 * Operations: set the button IO
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTA.0             mode switch
 *   PORTA.4             start pushbutton
 *   PORTA.5             stop pushbutton
 */

/* NOTE: Includes */
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
    IO_init();

    initDebug();

    while(1)
    {
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // set port A as all inputs
    DDRA  = 0x00;
    // turn all pullup resisistors
    PORTA = 0xFF;
}