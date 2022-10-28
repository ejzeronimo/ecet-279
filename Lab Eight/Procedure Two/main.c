/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 10/26/2022 2:23:36 PM
 * Author: Ethan Zeronik
 *
 * Operations: ADC using interrupts
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTA               LED bar
 *   PORTF.0             Potentiometer in
 */

/* NOTE: Includes */
#include <avr/io.h>

#include "AnalogToDigital.h"

/* NOTE: Custom Macros */
// TODO: None

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    ADC_initInterrupt();

    IO_init();

    sei();

    while(1)
    {
        // get the value and bit shift it right 2
        PORTA = ADC_getTenBitValueInterrupt(0) >> 2;
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // set portA as an output
    DDRA  = 0xFF;
    PORTA = 0x00;
}
