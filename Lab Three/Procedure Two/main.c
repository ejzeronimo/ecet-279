/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 9/14/2022 1:58 PM
 * Author: Ethan Zeronik
 *
 * Operations: rotates motor 2 revolutions in every mode of operation depending on input
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
    initDebug();

    IO_init();

    SM_init(&DDRA, &PORTA);

    while(1)
    {
        switch(PINA & (0xf0))
        {
            case 0x10:
            {
                SM_move((StepperMotorRunMode_t)0, 2);
            }
            break;
            case 0x20:
            {
                SM_move((StepperMotorRunMode_t)1, 2);
            }
            break;
            case 0x40:
            {
                SM_move((StepperMotorRunMode_t)2, 2);
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