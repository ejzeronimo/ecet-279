/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 11/9/2022 2:26:50 PM
 * Author: Ethan Zeronik
 *
 * Operations: uart to lcd
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTL               LCD data
 *   PORTD.0             LCD RS
 *   PORTD.1             LCD R/W
 *   PORTD.2             LCD E
 */

/* NOTE: Includes */
#include <avr/io.h>

#include "LiquidCrystalDisplay.h"
#include "Serial.h"

/* NOTE: Custom Macros */
// TODO: None

/* NOTE: Global Variables */
// buffer for uart
char    message[24]  = {0};
uint8_t messageIndex = 0;
uint8_t readFlag     = 0;

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);
// handler
void asyncGetHandler(char c);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();
    LCD_init();

    // init async uart and bind an interrupt handler
    SERIAL_uartInitAsync();
    SERIAL_uartAsyncGetHandler(&asyncGetHandler);

    sei();

    LCD_instruction(0x01);
    LCD_instruction(0x02);

    while(1)
    {
        if(readFlag)
        {
            LCD_instruction(0x01);
            LCD_instruction(0x02);
            LCD_sendString(message);

            readFlag = 0;
        }
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // port l all out
    DDRL  = 0xFF;
    PORTL = 0x00;

    // port d just out on the first three pins
    DDRD = 0x07;
}

void asyncGetHandler(char c)
{
    if(c != 0x0d && c != 0x0a && c != '\0')
    {
        if(messageIndex < 23)
        {
            // add to array
            message[messageIndex]     = c;
            message[messageIndex + 1] = '\0';

            messageIndex++;
        }
    }
    else
    {
        for(uint8_t i = 0; i < 24; i++)
        {
            message[i] = '\0';
        }

        messageIndex = 0;
    }

    // set update flag
    readFlag = 1;
}
