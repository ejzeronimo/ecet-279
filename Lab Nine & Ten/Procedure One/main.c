/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 11/9/2022 1:54 PM
 * Author: Ethan Zeronik
 *
 * Operations: lcd test
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
char message[] = {"Test"};

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();
    LCD_init(&DDRD, &PORTD, &DDRL, &PORTL);

    LCD_sendInstruction(0x01);
    _delay_ms(2); // can remove if use Busy Flag check

    LCD_sendInstruction(0x02);
    _delay_ms(2); // can remove if use Busy Flag check

    LCD_sendInstruction(0x86);
    _delay_ms(50);

    LCD_sendString(message);
    _delay_ms(50);

    LCD_sendInstruction(0xC6);
    _delay_us(50); // can remove if use Busy Flag check

    LCD_sendString(message);

    while(1)
    {
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // do nothing
}
