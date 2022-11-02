/*
 * FileName: Encoder.c
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
    // turn on the clock and direction inputs
    DDRD  |= 0x06;
    PORTD |= 0x06;

    // interupt 2 to enabled falling edge
    EIMSK |= (1 << INT2);
    EICRA |= (1 << ISC21);
}

uint8_t ENC_getValue(void)
{
    return value;
}

uint8_t ENC_getDirection(void)
{
    // cache the flag state
    uint8_t dir = direction;

    // reset the flag in the global
    direction = 0x00;

    // return the cached state
    return dir;
}

ISR(INT2_vect)
{
    // if pin is high
    if(PIND & 0x02)
    {
        // set the flag
        direction = 0x11;

        // increment if it won't overflow
        if(value < 255)
        {
            value++;
        }
    }
    else
    {
        // set the flag
        direction = 0x10;

        // decrement if it won't overflow
        if(value > 0)
        {
            value--;
        }
    }
}