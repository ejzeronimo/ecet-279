/*
 * CraneEeprom.c
 *
 * Created: 2022/11/5 10:03:43 am
 * Author: xiang82
 */

/* NOTE: Includes */
#include "CraneEeprom.h"

/* NOTE: Global function implementations */
void CRANE_eepromWriteChar(char ucData, uint16_t uiAddress)
{
    while(EECR & (1 << EEPE))
    {
        /* Wait for completion of previous write */
    }

    /* Set up address and Data Registers */
    EEAR = uiAddress;
    EEDR = ucData;

    /* Write logical one to EEMPE */
    // step 5. write 1 to EEMPE and 0 to EEPE
    EECR = (1 << EEMPE);

    /* Start EEPROM write by setting EEPE */
    // write EEPE within 4 clock cycles
    EECR |= (1 << EEPE);
}

char CRANE_eepromReadChar(uint16_t uiAddress)
{
    while(EECR & (1 << EEPE))
    {
        /* Wait for completion of previous write */
    };

    EEAR = uiAddress;
    EECR |= (1 << EERE);

    return EEDR;
}

void CRANE_eepromWriteString(char const * const ucData, uint16_t uiAddress)
{
    uint16_t n = 0;

    while(ucData[n] != '\0')
    {
        CRANE_eepromWriteChar(ucData[n], uiAddress);

        n++;
    }

    CRANE_eepromWriteChar('\0', uiAddress + 1);
}

void CRANE_eepromReadString(uint16_t uiAddress, char * const EEPROM_buf_ptr)
{
    char * pWorker = (char *)EEPROM_buf_ptr;

    // changed 0xFF to 0x00 because we are looking for end of string
    while(CRANE_eepromReadChar(uiAddress) != 0x00)
    {
        *pWorker = CRANE_eepromReadChar(uiAddress);

        pWorker++;
        uiAddress++;
    }
}