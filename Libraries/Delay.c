/*
 * FileName: Delay.c
 * Version: 1
 *
 * Created: 10/18/2022 7:26 PM
 * Author: Ethan Zeronik
 *
 * Operations: create a custom delay function
 */

/* NOTE: Includes */
#include "Delay.h"

#include <avr/io.h>

/* NOTE: Local declarations */
// TODO: None

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Local function implementations */
void DLY_init(void)
{
    // set counter to 0
    TCNT0 = 0;

    // normal mode
    TCCR0A = 0x00;
    TCCR0B = 0x00;

    OCR0A = 0;
}

void DLY_ms(double ms)
{
    uint8_t time = (((ms / 1000.0) * F_CPU) / 1024);

    if(time < 255)
    {
        OCR0A = time;

        // prescalar of 1024
        TCCR0B = 0x05;

        while((TIFR0 & (1 << OCF0A)) == 0)
        {
            // do nothing
        }

        // stop the timer
        TCCR0B = 0x00;
        // clear the overflow flag
        TIFR0 |= (1 << OCF0A);
        TCNT0 = 0;
        OCR0A = 0;
    }
    else
    {
        OCR0A = (((1 / 1000.0) * F_CPU) / 1024);

        // prescalar of 1024
        TCCR0B = 0x05;

        for(size_t i = 0; i < ms; i++)
        {
            while((TIFR0 & (1 << OCF0A)) == 0)
            {
                // do nothing
            }

            // stop the timer
            TCCR0B = 0x00;
            // clear the overflow flag
            TIFR0 |= (1 << OCF0A);
            TCNT0 = 0;
            OCR0A = 0;
        }
    }
}
