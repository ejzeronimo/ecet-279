/*
 * FileName: LiquidCrystalDisplay.c
 * Version: 1
 *
 * Created: 11/9/22 1:59 PM
 * Author: Ethan Zeronik
 *
 * Operations: lcd implementation
 */

/* NOTE: Includes */
#include "LiquidCrystalDisplay.h"

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define INSTR_WR 0
#define DATA_WR  1

/* NOTE: Local declarations */
// TODO: None

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Local function implementations */
void LCD_init(void)
{
    _delay_ms(35);         /* wait for more than 30mS after VDD rises to 4.5V */
    LCD_instruction(0x38); /* function set 8bits, 2line, display off */
    _delay_us(50);         /* wait for more than 39microS */
    LCD_instruction(0x0C); /* display on, cursor off, blink off */
    _delay_us(50);         /* wait for more than 39microS */
    LCD_instruction(0x01); /* display clear */
    _delay_ms(2);          /* wait for more than 1.53mS */
    LCD_instruction(0x06); /* entry mode set, increment mode */
}

void LCD_instruction(uint8_t i)
{
    PORTD = 0b00000000; /* Write instruction: RS = 0 E = 0, R/!W=0 (write) */

    PORTD = PORTD | 0x04; /* Take E HIGH (logic 1) */
    PORTL = i;
    _delay_us(50);        /* needs to be at least 30uS or no display - use 50 */
    PORTD = PORTD & 0x01; /* Take E LOW (logic 0) */
    _delay_us(50);        /* Delay REQUIRED */

    _delay_ms(5);
}

void LCD_sendChar(char c)
{
    PORTD = 0b00000001; /* Write instruction: RS = 0 E = 0, R/!W=0 (write) */

    PORTD = PORTD | 0x04; /* Take E HIGH (logic 1) */
    PORTL = c;
    _delay_us(50);        /* needs to be at least 30uS or no display - use 50 */
    PORTD = PORTD & 0x01; /* Take E LOW (logic 0) */
    _delay_us(50);        /* Delay REQUIRED */

    _delay_ms(5);
}

void LCD_sendString(char * s)
{
    PORTD = 0b00000001; /* write data: RS = 1 E = 0, R/!W=0 (write) */

    while(*s != '\0')
    {
        PORTD = PORTD | 0x04; /* Take E HIGH (logic 1) */
        PORTL = *s++;
        _delay_us(50);        /* needs to be at least 30uS or no display - use 50 */
        PORTD = PORTD & 0x01; /* Take E LOW (logic 0) */
        _delay_us(50);        /* Delay REQUIRED */
    }

    _delay_ms(5);
}