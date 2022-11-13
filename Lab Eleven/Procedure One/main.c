/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 11/8/2022 11:50:01 PM
 * Author: Ethan Zeronik
 *
 * Operations: basic eeprom
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTG0:2            control for the lcd
 *   PORTl               data buss for the lcd
 */

/* NOTE: Includes */
#include <avr/io.h>

#include "Eeprom.h"

/* NOTE: Custom Macros */
// TODO: None

/* NOTE: Global Variables */
uint16_t addr = 0x0100;

/* NOTE: Function prototypes */
// TODO: none

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    char const * const result = EEPROM_readString(addr);

    while(1)
    {
        // do nothing
    }
}

/* NOTE: Function implementations */
// TODO: none