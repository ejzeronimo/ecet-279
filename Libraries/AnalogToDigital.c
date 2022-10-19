/*
 * FileName: AnalogToDigital.c
 * Version: 1
 *
 * Created: 10/19/2022 12:47 AM
 * Author: Ethan Zeronik
 *
 * Operations: basic adc implementation
 */

/* NOTE: Includes */
#include "AnalogToDigital.h"

#include <avr/io.h>

/* NOTE: Local declarations */
// TODO: None

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Local function implementations */
void ADC_init(void)
{
    // ten bit one way mode
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);

    // 5v reference
    ADMUX = (1 << REFS0);

    ADCSRB = 0x00;
}

double ADC_getTenBitValue(uint16_t channel)
{
    uint16_t result = 0;

    // select the channel
    ADMUX = (ADMUX & 0xe0) | channel;
    ADCSRB = (ADCSRB & 0xf7) | (channel >> 2);

    // start conversion
    ADCSRA |= (1 << ADSC);

    // wait for conversion
    while((ADCSRA & (1 << ADSC)) == 1)
    {
        // do nothing
    }

    // save result
    result = ADCL;
    result = result | (ADCH << 8);

    return result / 1024.0;
}