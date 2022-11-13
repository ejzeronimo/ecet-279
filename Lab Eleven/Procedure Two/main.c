/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 11/12/2022 8:19:38 PM
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
#include <avr/interrupt.h>

#include "Eeprom.h"
#include "Serial.h"
#include "LiquidCrystalDisplay.h"

/* NOTE: Custom Macros */
// TODO: None

/* NOTE: Global Variables */
// address for the stored value
uint16_t addr         = 0x0100;
// buffer for uart
char     message[16]  = {0};
uint8_t  messageIndex = 0;
uint8_t  readFlag     = 0;

/* NOTE: Function prototypes */
// handler
void asyncGetHandler(char c);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    LCD_init(&DDRG, &PORTG, &DDRL, &PORTL);

    SERIAL_uartInitAsync(serialUsart0, 9600);
    SERIAL_uartAsyncGetHandler(serialUsart0, &asyncGetHandler);

    // clear, home, and move cursor
    LCD_sendInstruction(0x01);
    LCD_sendInstruction(0x02);
    LCD_sendInstruction(0x80);

    LCD_sendString("Last Saved:");

    char const * const result = EEPROM_readString(addr);

    // move to last line and print previous eeprom
    LCD_sendInstruction(0xc0);
    LCD_sendString(result);

    sei();

    SERIAL_uartSend(serialUsart0, "atmega booted!\n\r");
    SERIAL_uartSend(serialUsart0, result);

    while(1)
    {
        if(readFlag)
        {
            EEPROM_writeString(message, addr);

            SERIAL_uartSend(serialUsart0, "eeprom updated to:\n\r");
            SERIAL_uartSend(serialUsart0, message);

            readFlag = 0;
        }
    }
}

/* NOTE: Function implementations */
void asyncGetHandler(char c)
{
    if(c != 0x0d && c != 0x0a && c != '\0')
    {
        if(messageIndex < 15)
        {
            // add to array
            message[messageIndex]     = c;
            message[messageIndex + 1] = '\0';

            messageIndex++;
        }
    }
    else
    {
        // set update flag
        readFlag = 1;

        // reset message
        messageIndex = 0;
    }
}
