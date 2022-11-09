/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 11/9/2022 2:50:57 PM
 * Author: Ethan Zeronik
 *
 * Operations: lcd extra credit
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTL               LCD data
 *   PORTD.0             LCD RS
 *   PORTD.1             LCD R/W
 *   PORTD.2             LCD E
 */

/* NOTE: Includes */
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "LiquidCrystalDisplay.h"

/* NOTE: Custom Macros */
// TODO: None

/* NOTE: Global Variables */
uint8_t custom[] = {
    0b01000000,
    0b01001010,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01010001,
    0b01001110,
};

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();
    LCD_init();

    // set to accept in cgram
    LCD_instruction(0x40);

    LCD_sendString(custom);

    // clear and home
    LCD_instruction(0x01);
    LCD_instruction(0x02);

    LCD_sendChar(0x00);

    while(1)
    {
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
