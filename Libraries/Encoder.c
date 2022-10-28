/*
 * FileName: Delay.c
 * Version: 1
 *
 * Created: 10/26/2022 2:28 PM
 * Author: Ethan Zeronik
 *
 * Operations: encoder implementation
 */

/* NOTE: Includes */
#include "Encoder.h"

#include <avr/io.h>

/* NOTE: Local declarations */
// TODO: None

/* NOTE: Global Variables */
static uint8_t value     = 0;
// upper byte is flag lower is direction
static uint8_t direction = 0;

/* NOTE: Local function implementations */
void ENC_init(void)
{
    DDRD  = 0x06;
    PORTD = 0x06;

    EIMSK = (1 << INT2);
    EICRA = (1 << ISC21);
}

uint8_t ENC_getValue(void)
{
    return value;
}

uint8_t ENC_getDirection(void)
{
    uint8_t dir = direction;

    direction = 0x00;

    return dir;
}

ISR(INT2_vect)
{
    // if pin is high
    if(PIND & 0x02)
    {
        direction = 0x11;

        if(value < 255)
        {
            value++;
        }
    }
    // else decrement
    else
    {
        direction = 0x10;

        if(value > 0)
        {
            value--;
        }
    }
}