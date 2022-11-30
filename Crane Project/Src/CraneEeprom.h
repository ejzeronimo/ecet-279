/*
 * CraneEeprom.h
 *
 * Created: 11/2/2022 8:32:52 AM
 *  Author: xiang82
 */

#ifndef CraneEeprom_H_INCLUDED
#define CraneEeprom_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include <avr/io.h>

// write a charater to the given address
void CRANE_eepromWriteChar(char in, uint16_t addr);
// reads a character
char CRANE_eepromReadChar(uint16_t addr);
// write a string to the given address
void CRANE_eepromWriteString(char const * const in, uint16_t addr);
// reads a string from the address into the buffer
void CRANE_eepromReadString(uint16_t addr, char * const buf);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* CraneEeprom_H_INCLUDED */