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
 *   PORTF.0             pot in
 */

/* NOTE: Includes */
#include <avr/io.h>

#include "PulseWidthModulation.h"
#include "AnalogToDigital.h"
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
    ADC_init();

    initDebug();

    while(1)
    {
        if(PINA & 0x01)
        {
            PWM_enable();

            // set pwm to pot adc
            PWM_dutyCycle(ADC_getTenBitValue(0x00));
        }
        else
        {
            PWM_disable();

            // toggle led every 500 ms
            DLY_ms(500);
            PORTB = ~PORTB & 0x80;
        }
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // set port B.7 as an output
    DDRB  = 0x80;
    PORTB = 0x00;

    // set port A.0 as an input
    DDRA  = 0x00;
    PORTA = 0x01;

    // set port F.0 as an input
    DDRF  = 0x00;
    PORTF = 0x00;
}
