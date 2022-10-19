/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 10/19/2022 12:21:39 PM
 * Author: Ethan Zeronik
 *
 * Operations: ADC to PWM
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTB.5             PWM output
 *   PORTB.7             LED13 active high
 *   PORTA.0             mode switch
 *   PORTA.4             start pushbutton
 *   PORTA.5             stop pushbutton
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
        if()
        {
            PWM_enable();
            PWM_dutyCycle(.5);
        }
        else
        {
            PWM_disable();
            DLY_ms(500);

            PORTB = ~(0x80 & PORTB);
        }
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // set port B.7 as an output
    DDRB  = 0x80;
    PORTB = 0x00;

    // set port A.45 as an inputs
    DDRA  = ~0x30;
    PORTA = 0x30;
}
