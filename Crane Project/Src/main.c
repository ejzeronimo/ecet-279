/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 11/11/2022 8:46:12 AM
 * Author: Ethan Zeronik
 *
 * Operations: crane project source code
 *
 * Hardware:
 *   Atmega2560          micro controller
 */

/* NOTE: Includes */
// standard include for the atmega program
#include <avr/io.h>

// adds stepper motor module (1/6)
#include "StepperMotor.h"

// adds pwm for servo motors (2/6)
#include "PulseWidthModulation.h"

/* NOTE: Custom Macros */
// the three main states of the application
#define calibrateState 0
#define actionState    1
#define recordState    2

// custom buttons & switches
#define limitSwitch (PINC & 0x10)
#define rightButton (PINA & 0x02)
#define leftButton  (PINA & 0x01)

/* NOTE: Global Variables */
// the state of the application
uint8_t applicationState = calibrateState;

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();

    // in this case, 1 is CCW, and 0 is CW
    // pass in the port and register we want to use for the motor
    SM_init(&DDRC, &PORTC);

    while(1)
    {
        /*
            three modes, record, calibrate, and default

            calibrate should rerun the homing during init

            record needs to save the the actions to a LL and store them in eeprom

            action needs to be able to move the crane
        */

        switch(applicationState)
        {
            // the action case
            case actionState:
            {
                if(rightButton)
                {
                    SM_moveTime(stepperModeHalf, 0, 90, 3);
                }

                if(leftButton)
                {
                    SM_moveTime(stepperModeHalf, 1, 90, 3);
                }
            }
            break;

            // the default case will be the home case
            case calibrateState:
            default:
            {
                // move CW for one second to ensure the limit switch is not set
                SM_moveTime(stepperModeHalf, 0, 1000, 3);

                // while not hitting the switch
                while(!limitSwitch)
                {
                    // then move CCW a bit at the time until we hit the limit switch
                    SM_moveTime(stepperModeHalf, 1, 12, 3);
                }

                // then move 30 degrees back to center the arm
                SM_movePosition(stepperModeHalf, .0015);

                // then set it to action state
                applicationState = actionState;
            }
            break;
        }
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // port c.4 is the limit switch
    DDRC  = 0x00;
    PORTC = 0x10;

    // port a.0-1 are for left and right
    DDRA  = 0x00;
    PORTA = 0x03;
}