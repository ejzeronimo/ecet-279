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
 *   PORTA               LED bar
 *   PORTC               Button/Switches
 */

/* NOTE: Includes */
#include <avr/io.h>

#include "Serial.h"
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
    // init async uart and bind an interrupt handler
    SERIAL_uartInit();

    char * result = EEPROM_readString(0x0100);

    while(1)
    {
        // do nothing
    }
}

/* NOTE: Function implementations */
// TODO: none