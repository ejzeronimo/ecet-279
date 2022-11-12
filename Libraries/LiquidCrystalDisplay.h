/*
 * FileName: LiquidCrystalDisplay.h
 * Version: 1
 *
 * Created: 11/9/22 1:59 PM
 * Author: Ethan Zeronik
 *
 * Operations: lcd definition
 */

#ifndef LiquidCrystalDisplay_h_INCLUDED
#define LiquidCrystalDisplay_h_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#pragma message("WARNING: this module uses lower 3 bits of control and entire data buffer assigned")

#include <stdint.h>

/* NOTE: Custom Types */
// custom type for charcater definition
typedef uint8_t LcdCustomCharacter_t[8];
// typing for the allowed character addresses
typedef enum LcdCharacterAddress_t
{
    // the first charcater slot
    lcdFirstSlot   = 0x40,
    // the second charcater slot
    lcdSecondSlot  = 0x48,
    // the third charcater slot
    lcdThirdSlot   = 0x50,
    // the fourth charcater slot
    lcdFourthSlot  = 0x58,
    // the fifth charcater slot
    lcdFifthSlot   = 0x60,
    // the sixth charcater slot
    lcdSixthSlot   = 0x68,
    // the seventh charcater slot
    lcdSeventhSlot = 0x70,
    // the eigth charcater slot
    lcdEighthSlot  = 0x78,
} LcdCharacterAddress_t;

/* NOTE: Function prototypes */
// init for the lcd
// the data register is entirely used
// the control register is only used for the lower 3 bits
void LCD_init(uint8_t volatile * const pControlRegister, uint8_t volatile * const pControlPort, uint8_t volatile * const pDataRegister, uint8_t volatile * const pDataPort);
// sends the given instruction
void LCD_sendInstruction(uint8_t input);
// send a single char
void LCD_sendChar(char c);
// send a string
void LCD_sendString(char const * const pData);
// creates the custom character at the given address
void LCD_createCharacter(LcdCharacterAddress_t address, LcdCustomCharacter_t custom);
// gets the created character
// to be used as a char in a astring
char LCD_getCharacter(LcdCharacterAddress_t address);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // LiquidCrystalDisplay_h_INCLUDED