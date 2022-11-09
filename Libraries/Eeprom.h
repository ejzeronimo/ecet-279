/*
 * FileName: Eeprom.h
 * Version: 1
 *
 * Created: 11/8/2022 11:54 PM
 * Author: Ethan Zeronik
 *
 * Operations: eeprom definition
 */

#ifndef Eeprom_h_INCLUDED
#define Eeprom_h_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>

/* NOTE: Custom Types */
// TODO: none

/* NOTE: Function prototypes */
// write a charater to the given address
void   EEPROM_writeChar(char in, uint16_t addr);
// read a character
char   EEPROM_readChar(uint16_t addr);
// write a string to the given address
void   EEPROM_writeString(char * in, uint16_t addr);
// read a string from the address
char * EEPROM_readString(uint16_t addr);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // Eeprom_h_INCLUDED