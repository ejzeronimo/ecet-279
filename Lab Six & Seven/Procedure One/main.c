/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 10/11/2022 5:36:00 PM
 * Author: Ethan Zeronik
 *
 * Operations: TODO: fill in!
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTx.x             LED13 active high
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
    IO_init();

    initDebug();

    while(1)
    {
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // TODO: Implement!
}