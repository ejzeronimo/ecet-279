/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 10/18/2022 8:55:13 PM
 * Author: Ethan Zeronik
 *
 * Operations: make a basic PWM controller
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTA.4             start pushbutton
 *   PORTA.5             stop pushbutton
 *   PORTB.5             PWM output
 */

/* NOTE: Includes */
#include <avr/io.h>

#include "PulseWidthModulation.h"
#include "Delay.h"
#include "Debugger.h"

/* NOTE: Custom Macros */
// TODO: None

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);
// makes a ramp
void rampUpDelayWithSteps(double start, double end, uint16_t duration, uint8_t stepCount);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();
    PWM_init();
    DLY_init();

    initDebug();

    while(1)
    {
        while((PINA & 0x10) == 0)
        {
            // do nothing
        }

        PWM_enable();

        rampUpDelayWithSteps(.1, .5, 8000, 5);

        while((PINA & 0x20) == 0)
        {
            // do nothing
        }

        PWM_disable();
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // set port B.7 as an output
    DDRA  = ~0x30;
    PORTA = 0x30;
}

void rampUpDelayWithSteps(double start, double end, uint16_t duration, uint8_t stepCount)
{
    double stepIncrement = (end - start) / (stepCount - 1);

    for(size_t i = 0; i < stepCount; i++)
    {
        DLY_ms(duration / stepCount);
        PWM_dutyCycle((stepIncrement * i) + start);
    }
}