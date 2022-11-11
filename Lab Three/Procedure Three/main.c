/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 9/14/2022 3:00 PM
 * Author: Ethan Zeronik
 *
 * Operations: moves the motor by the given amount in degrees
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTA.0-3           button inputs (pullup resistors on)
 *   PORTA.4-7           led output
 */

/* NOTE: Includes */
#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "StepperMotor.h"
#include "Debugger.h"

/* NOTE: Custom Macros */
#define Angle 180

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    initDebug();

    IO_init();

    SM_init(&DDRA, &PORTA);

    while(1)
    {
        switch(PINA & (0xf0))
        {
            case 0x10:
            {
                SM_movePosition(stepperModeWave, Angle);
            }
            break;
            case 0x20:
            {
                SM_movePosition(stepperModeFull, Angle);
            }
            break;
            case 0x40:
            {
                SM_movePosition(stepperModeHalf, Angle);
            }
            break;
            default:
                break;
        }
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // the bottom nibble is the motor output while the top is button input
    DDRA  = 0x00;
    // turn on pullup resisitors on the top nibble
    PORTA = 0xf0;
}