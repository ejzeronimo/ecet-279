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

#pragma message("WARNING: this module uses PORTD0:2 for control and PORTL for data")

#include <stdint.h>

/* NOTE: Custom Types */
// TODO: None

/* NOTE: Function prototypes */
// init for the lcd
void LCD_init(void);
// sends the given instruction
void LCD_instruction(uint8_t i);
// send a single char
void LCD_sendChar(char c);
// send a string
void LCD_sendString(char  * s);


#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // LiquidCrystalDisplay_h_INCLUDED