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

/* NOTE: Global Variables */
// instance pointer to the control logic
static uint8_t * sContolPort;
// instance pointer to the data port
static uint8_t * sDataPort;

/* NOTE: Local function implementations */
void LCD_init(uint8_t volatile * const pControlRegister, uint8_t volatile * const pControlPort, uint8_t volatile * const pDataRegister, uint8_t volatile * const pDataPort)
{
    // configure port register and turn off port
    *pDataRegister |= 0xff;
    *pDataPort = 0x00;

    // configure port register and turn off port
    *pControlRegister |= 0x07;
    *pControlPort = (*pControlPort & 0xf8) | 0x00;

    sContolPort = (uint8_t *)pControlPort;
    sDataPort   = (uint8_t *)pDataPort;

    // wait for lcd to power up
    _delay_ms(35);

    // set lcd to 8 bits, 2 lines, display off
    LCD_sendInstruction(0x38);
    _delay_us(50);

    // set lcd to display on, cursor off, blink off
    LCD_sendInstruction(0x0C);
    _delay_us(50);

    // clear the display
    LCD_sendInstruction(0x01);
    _delay_ms(2);

    // incrmement mode
    LCD_sendInstruction(0x06);
}

void LCD_sendInstruction(uint8_t input)
{
    // set controls to RS = 0 E = 0, R/!W=0 then take E high
    *sContolPort = (*sContolPort & 0xf8) | 0x00;
    *sContolPort |= 0x04;

    // send data then delay for at least 50us
    *sDataPort = input;
    _delay_us(50);

    // take E low
    *sContolPort = *sContolPort & 0xf9;

    _delay_ms(5);
}

void LCD_sendChar(char c)
{
    // set controls to RS = 1 E = 0, R/!W=0 then take E high
    *sContolPort = (*sContolPort & 0xf8) | 0x01;
    *sContolPort = *sContolPort | 0x04;

    // send data then delay for at least 50us
    *sDataPort = c;
    _delay_us(50);

    // take E low
    *sContolPort = (*sContolPort & 0xf8) | 0x01;

    _delay_ms(5);
}

void LCD_sendString(char const * const pData)
{
    char * localPointer = (char * const)pData;

    // set controls to RS = 1 E = 0, R/!W=0
    *sContolPort = (*sContolPort & 0xf8) | 0x01;

    while(*localPointer != '\0')
    {
        // take E high
        *sContolPort = *sContolPort | 0x04;

        // send data then delay for at least 50us
        *sDataPort = *localPointer++;
        _delay_us(50);

        // take E low
        *sContolPort = (*sContolPort & 0xf8) | 0x01;

        _delay_us(50); /* Delay REQUIRED */
    }

    _delay_ms(5);
}

void LCD_createCharacter(LcdCharacterAddress_t address, LcdCustomCharacter_t custom)
{
    // make sure that the character is correct and valid
    LcdCustomCharacter_t safety = {
        0x40 | (custom[0] & 0x1f),
        0x40 | (custom[1] & 0x1f),
        0x40 | (custom[2] & 0x1f),
        0x40 | (custom[3] & 0x1f),
        0x40 | (custom[4] & 0x1f),
        0x40 | (custom[5] & 0x1f),
        0x40 | (custom[6] & 0x1f),
        0x40 | (custom[7] & 0x1f),
    };

    // set the address of the cgram (must be between 64 - 127)
    LCD_sendInstruction(address);

    // send the sterile character
    LCD_sendString((char const * const)safety);
}

char LCD_getCharacter(LcdCharacterAddress_t address){
    return (address - 64) /8;
}