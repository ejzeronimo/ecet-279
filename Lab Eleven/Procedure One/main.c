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
// TODO: None

/* NOTE: Function prototypes */
// TODO: none

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    // init async uart and bind an interrupt handler
    SERIAL_uartInit();

    char format[] = {EEPROM_readChar(0x0100), '\n', '\r', '\0'};
    SERIAL_uartSend((char const * const)format);

    while(1)
    {
        char result = SERIAL_uartGetSync();
        
        // 0d 0a 00
        if (result != 0x00)
        {
            EEPROM_writeChar(result, 0x0100);
        }
        
    }
}

/* NOTE: Function implementations */
// TODO: none