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
LcdCustomCharacter_t smile = {
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
};

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();
    LCD_init(&DDRG, &PORTG, &DDRL, &PORTL);

    LCD_createCharacter(lcdFirstSlot, smile);

    // clear and home
    LCD_sendInstruction(0x01);
    LCD_sendInstruction(0x02);

    LCD_sendChar(LCD_getCharacter(lcdFirstSlot));

    while(1)
    {
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // do nothing
}
