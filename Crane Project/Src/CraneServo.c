/*
 * CraneServo.c
 *
 * Created: 2022/11/17 下午 09:01:15
 *  Author: Yu-Hung (Thomas) Wang
 */

/* NOTE: Includes */
#include "CraneServo.h"

/* NOTE: Global function implementations */
void CRANE_initServos(void)
{
    // set up PORTB.5 as an output and 0V
    DDRB |= 0x60;
    PORTB |= PORTB & ~0x60;

    // 5000 @ 64
    // set frequency to 50hz
    ICR1 = 5000;

    // fast pwm set on compare
    TCCR1A = 0x02;
    // prescaler set to 64
    TCCR1B = 0x1B;
}

void CRANE_setServoPosition(uint8_t servo, uint8_t position)
{
    if(servo == 0)
    {
        OCR1A = (uint32_t)position * 250 / 255 + 250;
    }
    else if(servo == 1)
    {
        OCR1B = (uint32_t)position * 250 / 255 + 250;
    }
}

void CRANE_startServos(void)
{
    TCCR1A |= 0xA0;
}

void CRANE_stopServos(void)
{
    TCCR1A = (TCCR1A & ~0x80);
}