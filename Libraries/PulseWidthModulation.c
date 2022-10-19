/*
 * FileName: PulseWidthModulation.c
 * Version: 1
 *
 * Created: 10/18/2022 10:14 PM
 * Author: Ethan Zeronik
 *
 * Operations: create a custom pwm function
 */

/* NOTE: Includes */
#include "PulseWidthModulation.h"

#include <avr/io.h>

/* NOTE: Local declarations */
// TODO: None

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Local function implementations */
void PWM_init(void)
{
    // set up PORTB.5 as an output and 0V
    DDRB |= 0x20;
    PORTB |= PORTB & ~0x20;

    // 512 @ 8
    // set frequency to 3900hz
    ICR1 = 512;

    // fast pwm set on compare
    TCCR1A = 0x02;
    // prescaler set to 8
    TCCR1B = 0x1a;
}

void PWM_dutyCycle(double percent)
{
    OCR1A = percent * 512;
}

void PWM_enable(void)
{
    TCCR1A |= 0x80;
}

void PWM_disable(void)
{
    OCR1A = 0x00;

    TCCR1A = (TCCR1A & ~0x80);
}
