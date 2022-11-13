/*
 * FileName: Eeprom.c
 * Version: 1
 *
 * Created: 11/8/2022 11:54 PM
 * Author: Ethan Zeronik
 *
 * Operations: eeprom implementation
 */

/* NOTE: Includes */
#include "Eeprom.h"

#include <avr/io.h>
#include <stdlib.h>

/* NOTE: Local declarations */
// reads blob from the given address
uint8_t read(uint16_t address);
// writes blob to the given address
void    write(uint8_t data, uint16_t address);

/* NOTE: Function implementations */
void EEPROM_writeChar(char in, uint16_t addr)
{
    write(in, addr);
}

char EEPROM_readChar(uint16_t addr)
{
    return (char)read(addr);
}

void EEPROM_writeString(char const * const in, uint16_t addr)
{
    uint16_t i = 0;

    while(in[i] != '\0')
    {
        write(in[i], addr + i);

        i++;
    }

    // add in a null terminator
    write('\0', addr + i);
}

char const * const EEPROM_readString(uint16_t addr)
{
    char * buf = malloc(sizeof(char) * 256);
    uint16_t     i   = 0;

    while(read(addr + i) != '\0')
    {
        // read into buffer
        buf[i] = read(addr + i);

        i++;
    }

    // add in the null terminator and resize
    buf[i] = '\0';
    buf    = realloc(buf, sizeof(char) * (i + 1));

    return buf;
}

/* NOTE: Local function implementations */
uint8_t read(uint16_t address)
{
    // wait for read completion
    while(EECR & (1 << EEPE))
    {
        // do nothing
    }

    // set address
    EEAR = address;

    // read and return
    EECR |= (1 << EERE);
    return EEDR;
}

void write(uint8_t data, uint16_t address)
{
    // wait for write completion
    while(EECR & (1 << EEPE))
    {
        // do nothing
    }

    // set address and data
    EEAR = address;
    EEDR = data;

    // enable master program and start
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}