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

#if !defined(F_CPU)
    #define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

/* NOTE: Global Variables */
// internal tick tracker
static uint16_t tick = 0;

/* NOTE: Local function implementations */
void DLY_init(void)
{
    // reset counter to 0
    TCNT0 = 0;

    // normal mode
    TCCR0A = 0x00;
    TCCR0B = 0x00;

    OCR0A = 0;
}

void DLY_initInterrupt(void)
{
    // interrupt mode
    TCCR0A = 0x00;
    TCCR0B = 0x04;
    TCNT0  = 0;
    OCR0A  = 62;
    TIMSK0 = (1 << OCIE0A);
}

void DLY_ms(double ms)
{
    uint32_t time = (((ms / 1000.0) * F_CPU) / 1024);

    if(ms <= 16)
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

        for(uint32_t i = 0; i < ms; i++)
        {
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
        }

        OCR0A = 0;
    }
}

uint16_t DLY_getTick(void)
{
    return tick;
}

void DLY_setTick(uint16_t t)
{
    tick = t;
}

ISR(TIMER0_COMPA_vect)
{
    TCNT0 = 0;
    tick++;
}
