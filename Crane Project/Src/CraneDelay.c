/*
 * CraneDelay.c
 *
 * Created: 10/5/2022 8:47:57 AM
 * Author : Alex Weyer
 */

/* NOTE: Includes */
#include "CraneDelay.h"

#if !defined(F_CPU)
    #define F_CPU 16000000UL
#endif

/* NOTE: Local declarations */
// variable to increment and use for delay
volatile uint16_t tick;

/* NOTE: Global function implementations */
void CRANE_initTimer(void)
{
    TCNT0  = 6;
    TCCR0A = 0;
    TCCR0B = 0x03;
    TIMSK0 = (1 << TOIE0);
}

uint16_t CRANE_tick(void)
{
    return tick;
}

void CRANE_delayMs(uint16_t ms){
    uint16_t desiredTick = tick + ms;

    while (desiredTick != tick)
    {
        // do nothing
    }
}

/* NOTE: Local function implementations */
ISR(TIMER0_OVF_vect)
{
    TCNT0 = 6;
    tick++;
}