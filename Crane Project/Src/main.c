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
#include <avr/interrupt.h>

// adds stepper motor module (1/6)
#include "StepperMotor.h"
// adds adc for input potentiometers (2/6)
#include "AnalogToDigital.h"
// adds pwm for servo motors (3/6)
#include "CraneServo.h"
// adds UART for communication (4/6)
#include "Serial.h"

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
uint8_t          applicationState  = calibrateState;
// the input buffer for serial
volatile char    serialBuffer[128] = {0};
// the index of the last set char in the serial buffer
volatile uint8_t serialBufferIndex = 0;
// flag to show serial buffer is ready
volatile uint8_t serialReadFlag    = 0;

/* NOTE: Function prototypes */
// inits IO ports
void    IO_init(void);
// handler
void asyncSerialRxHandler(char c);
// compares to strings
// if the match it returns one, else 0
uint8_t stringCompare(char const * const pStrOne, char const * const pStrTwo);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();

    // init the ADC and the servo control
    ADC_init();
    CRANE_initServos();

    // in this case, 1 is CCW, and 0 is CW
    // pass in the port and register we want to use for the motor
    SM_init(&DDRC, &PORTC);

    // turn on the serial on port 0 at 9600 baud
    SERIAL_uartInitAsync(serialUsart0, 9600);
    SERIAL_uartAsyncGetHandler(serialUsart0, &asyncSerialRxHandler);
    SERIAL_uartSend(serialUsart0, "Crane online\r\n");

    sei();

    while(1)
    {
        /*
            three modes, record, calibrate, and default

            calibrate should rerun the homing during init

            record needs to save the the actions to a LL and store them in eeprom

            action needs to be able to move the crane
        */

        if(serialReadFlag)
        {
            if(stringCompare(serialBuffer, "password") && (applicationState != recordState))
            {
                SERIAL_uartSend(serialUsart0, "Password entered, starting config mode...\r\n");

                applicationState = recordState;
            }

            serialReadFlag = 0;
        }

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

                CRANE_startServos();

                CRANE_setServoPosition(0, 255 * ADC_getTenBitValue(0));
                CRANE_setServoPosition(1, 255 * ADC_getTenBitValue(1));
            }
            break;

            // the record case
            case recordState:
            {
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
                SM_movePosition(stepperModeHalf, 30);

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

void asyncSerialRxHandler(char c)
{
    if(c != '\r' && c != '\n' && c != '\0' && (serialBufferIndex < 127))
    {
        // add to array
        serialBuffer[serialBufferIndex]     = c;
        serialBuffer[serialBufferIndex + 1] = '\0';

        serialBufferIndex++;
    }
    else
    {
        // set update flag
        serialReadFlag    = 1;
        // reset message index
        serialBufferIndex = 0;
    }
}

uint8_t stringCompare(char const * const pStrOne, char const * const pStrTwo)
{
    uint8_t i = 0;

    // while string one still has data
    do
    {
        if(*(pStrOne + i) == *(pStrTwo + i))
        {
            // increment
            i++;
        }
        else
        {
            // exit
            return 0;
        }
    } while((*(pStrOne + i) != '\0') && (*(pStrTwo + i) != '\0'));

    // made it out of the loop
    return 1;
}