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
 *   Port L              LED reposnse bar
 *   Port C.4            stepper motor limit switch
 *   Port C.0-3          stepper motor
 *   Port A.0-3          button inputs
 *   Port B.5-6          servo outputs       
 */

/* NOTE: Includes */
// standard include for the atmega program
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdio.h>

// adds stepper motor module (1/6)
#include "StepperMotor.h"
// adds adc for input potentiometers (2/6)
#include "AnalogToDigital.h"
// adds pwm for servo motors (3/6)
#include "CraneServo.h"
// adds UART and Bluetooth for communication (4/6)
#include "CraneCommunication.h"
// adds EEPROM read/write for persistant data storage (5/6)
#include "CraneEeprom.h"
// adds custom delay functions (6/6)
#include "CraneDelay.h"

/* NOTE: Types and Structs */
// type to hold data for serial connections
typedef struct connectionBuffer_t
{
    // the buffer to store the data in
    char    buffer[128];
    // the current index of the last set byte in the buffer
    uint8_t index;
    // whether or not the buffer is done being written to
    uint8_t readFlag;
} connectionBuffer_t;
// type to hold the position of the servos and motor
typedef struct cranePosition_t
{
    // motor position in steps
    int16_t motorTicks;
    // arm position in 0-255 ticks
    uint8_t armTicks;
    // plunger position in 0-255 ticks
    uint8_t plungerTicks;
} cranePosition_t;

/* NOTE: Custom Macros */
// the three main states of the application
#define calibrateState 0
#define actionState    1
#define recordState    2

// command definition
#define recordModeCommand    "Calibrate"
#define calibrateModeCommand "Reset"
#define runCommand           "Run"
#define recordCommand        "Record"
#define getCommand           "Get"

// custom buttons & switches
#define limitSwitch     (PINC & 0x10)
#define leftButton      (PINA & 0x01)
#define rightButton     (PINA & 0x02)
#define recordButton    (PINA & 0x04)
#define calibrateButton (PINA & 0x08)

// custom led bar
#define stateLed PORTL

// custom servos ids
#define armServo     0
#define plungerServo 1

// defined values for where default spots of the servo are
#define armStartPosition     150
#define plungerStartPosition 230

// record information
#define recordLength  6
#define eepromAddress 0x0555

/* NOTE: Global Variables */
// the state of the application
uint8_t         applicationState = calibrateState;
// the state of all the moving parts of the crane
cranePosition_t craneState       = {
          .motorTicks   = 0,
          .armTicks     = 0,
          .plungerTicks = 0,
};
// the EEPROM recorded moves to make
cranePosition_t             recordedMoves[recordLength];
// the moves to save to eemprom
cranePosition_t             currentMoves[recordLength];
// current spot in the instance moves
uint8_t                     currentMoveIndex = 0;
// the input buffer for serial
volatile connectionBuffer_t serialInputData  = {
     .buffer   = {0},
     .index    = 0,
     .readFlag = 0,
};
// the input buffer for bluetooth
volatile connectionBuffer_t bluetoothInputData = {
    .buffer   = {0},
    .index    = 0,
    .readFlag = 0,
};

/* NOTE: Function prototypes */
// inits IO ports
void    IO_init(void);
// gets the saved values from the eeprom
void    CRANE_getMovesFromEeprom(uint16_t addr);
// saves the current recorded moves to eeprom
void    CRANE_saveMovesToEeprom(uint16_t addr);
// takes a string and checks if the buffer matches the value exactly
// strictmode resets readflag
// returns 1 if true, else 0
uint8_t doesBufferMatch(volatile connectionBuffer_t buf, uint8_t strictMode, char const * const pStr);
// compares to strings
// if the match it returns 1, else 0
uint8_t stringCompare(char const * const pStrOne, char const * const pStrTwo);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    // start the gpio
    IO_init();

    // init the ADC and the servo control
    ADC_init();
    CRANE_initServos();
    CRANE_startServos();

    // in this case, 1 is CCW, and 0 is CW
    // pass in the port and register we want to use for the motor
    SM_init(&DDRC, &PORTC);

    // get the stored recorded data from the EEPROM
    CRANE_getMovesFromEeprom(eepromAddress);

    // turn on the serial on port 0 at 9600 baud
    CRANE_initSerial(9600);
    CRANE_sendSerial("Crane online\r\n");

    // turn on the blutooth connection
    CRANE_initBluetooth(9600);
    CRANE_sendBluetooth("Crane online\r\n");

    // start the delay timer
    CRANE_initTimer();

    // start global interrupts
    sei();

    while(1)
    {
        if(bluetoothInputData.readFlag)
        {
            CRANE_sendSerial(bluetoothInputData.buffer);
        }

        // DEBUG: get the state
        // if we get the command for record mode and we are not already in record mode
        if((doesBufferMatch(serialInputData, 0, recordModeCommand) && (applicationState != recordState)) || (doesBufferMatch(bluetoothInputData, 0, recordModeCommand) && (applicationState != recordState)) || (recordButton && (applicationState != recordState)))
        {
            CRANE_sendSerial("Entering record mode...\r\n");
            CRANE_sendBluetooth("Entering record mode...\r\n");

            applicationState            = recordState;
            serialInputData.readFlag    = 0;
            bluetoothInputData.readFlag = 0;

            while(recordButton)
            {
                // do nothing until we let go
            }
        }
        // check for reset command
        else if((doesBufferMatch(serialInputData, 0, calibrateModeCommand) && (applicationState != calibrateState)) || (doesBufferMatch(bluetoothInputData, 1, calibrateModeCommand) && (applicationState != calibrateState)) || (calibrateButton && (applicationState != calibrateState)))
        {
            CRANE_sendSerial("Resetting...\r\n");
            CRANE_sendBluetooth("Resetting...\r\n");

            applicationState            = calibrateState;
            serialInputData.readFlag    = 0;
            bluetoothInputData.readFlag = 0;

            while(calibrateButton)
            {
                // do nothing until we let go
            }
        }

        // display state on leds
        stateLed = (stateLed & 0xfc) | applicationState;

        // DEBUG: check for the get command
        if(doesBufferMatch(serialInputData, 0, getCommand) || doesBufferMatch(bluetoothInputData, 0, getCommand))
        {
            char response[96];
            sprintf(response, "Motor is at %i, Plunger is at %u, Arm is at %u\r\n", craneState.motorTicks, craneState.plungerTicks, craneState.armTicks);

            serialInputData.readFlag    = 0;
            bluetoothInputData.readFlag = 0;

            CRANE_sendSerial(response);
            CRANE_sendBluetooth(response);
        }

        // main application switch case
        switch(applicationState)
        {
            // DEBUG: the action case
            case actionState:
            {
                if(leftButton || doesBufferMatch(serialInputData, 0, runCommand) || doesBufferMatch(bluetoothInputData, 0, runCommand))
                {
                    // HACK: potential multiple select
                    serialInputData.readFlag    = 0;
                    bluetoothInputData.readFlag = 0;

                    for(uint8_t i = 0; i < recordLength; i++)
                    {
                        char response[32];
                        sprintf(response, "Running recorded step %u...\r\n", i + 1);

                        CRANE_sendSerial(response);
                        CRANE_sendBluetooth(response);
                        stateLed = (stateLed & 0x03) | 1 << (i + 2);

                        // calculate the relative movement of the arm
                        int16_t moveSteps = recordedMoves[i].motorTicks - craneState.motorTicks;

                        // move motor
                        if(moveSteps > 0)
                        {
                            SM_moveStepsSigned(stepperModeHalf, 0, moveSteps);
                        }
                        else if(moveSteps < 0)
                        {
                            SM_moveStepsSigned(stepperModeHalf, 1, -1 * moveSteps);
                        }

                        // lerp!
                        for(uint8_t j = 1; j < 101; j++)
                        {
                            CRANE_setServoPosition(armServo, craneState.armTicks + ((recordedMoves[i].armTicks - craneState.armTicks) * ((float)j / 100)));
                            CRANE_delayMs(5);
                        }

                        for(uint8_t j = 1; j < 101; j++)
                        {
                            CRANE_setServoPosition(plungerServo, craneState.plungerTicks + ((recordedMoves[i].plungerTicks - craneState.plungerTicks) * ((float)j / 100)));
                            CRANE_delayMs(5);
                        }

                        // set our state
                        craneState.motorTicks   = recordedMoves[i].motorTicks;
                        craneState.armTicks     = recordedMoves[i].armTicks;
                        craneState.plungerTicks = recordedMoves[i].plungerTicks;

                        CRANE_delayMs(100);
                    }

                    // reset state led
                    stateLed = (stateLed & 0x03);

                    while(leftButton)
                    {
                        // do nothing until we let go
                    }
                }
            }
            break;

            // DEBUG: the record case
            case recordState:
            {
                uint8_t armPosition     = 255 * ADC_getTenBitValue(0);
                uint8_t plungerPosition = 255 * ADC_getTenBitValue(1);
                int16_t moveSteps       = 8;

                // display the step we are recording
                stateLed = (stateLed & 0x03) | 1 << (currentMoveIndex + 2);

                // manually move the motor
                if(rightButton)
                {
                    SM_moveStepsSigned(stepperModeHalf, 0, moveSteps);
                    CRANE_delayMs(10);
                }
                else if(leftButton)
                {
                    SM_moveStepsSigned(stepperModeHalf, 1, moveSteps);
                    CRANE_delayMs(10);

                    // negate for current position
                    moveSteps *= -1;
                }
                else
                {
                    moveSteps = 0;
                }

                // manually move the servos
                CRANE_setServoPosition(armServo, armPosition);
                CRANE_setServoPosition(plungerServo, plungerPosition);

                // update the current positions
                craneState.motorTicks += moveSteps;
                craneState.armTicks     = armPosition;
                craneState.plungerTicks = plungerPosition;

                // if we press the record button save the position
                if(recordButton || doesBufferMatch(serialInputData, 0, recordCommand) || doesBufferMatch(bluetoothInputData, 0, recordCommand))
                {
                    serialInputData.readFlag    = 0;
                    bluetoothInputData.readFlag = 0;

                    char response[64];
                    sprintf(response, "Recording step %u out of 6...\r\n", currentMoveIndex + 1);

                    CRANE_sendSerial(response);
                    CRANE_sendBluetooth(response);

                    sprintf(response, "Recorded {%i,%u,%u}...\r\n", craneState.motorTicks, craneState.plungerTicks, craneState.armTicks);

                    CRANE_sendSerial(response);
                    CRANE_sendBluetooth(response);

                    // reset state led
                    stateLed = (stateLed & 0x03);

                    if(currentMoveIndex < recordLength)
                    {
                        currentMoves[currentMoveIndex++] = craneState;
                    }

                    if(currentMoveIndex >= recordLength)
                    {
                        CRANE_saveMovesToEeprom(eepromAddress);
                        currentMoveIndex = 0;

                        // done recording, back to action state after zeroing
                        // home ---> action(play)
                        applicationState = calibrateState;
                    }

                    while(recordButton)
                    {
                        // do nothing until we let go
                    }
                }
            }
            break;

            // DEBUG: the default case will be the home case
            case calibrateState:
            default:
            {
                // 0 is up 255 is down
                CRANE_setServoPosition(armServo, armStartPosition);
                // 255 is close 0 is open
                CRANE_setServoPosition(plungerServo, plungerStartPosition);

                // move CW for one second to ensure the limit switch is not set
                SM_moveTime(stepperModeHalf, 0, 1000, 3);

                // while not hitting the switch
                while(!limitSwitch)
                {
                    // then move CCW a bit at the time until we hit the limit switch
                    SM_moveStepsSigned(stepperModeHalf, 1, 24);
                }

                // then move 30 degrees back to center the arm
                SM_movePosition(stepperModeHalf, 35);

                // set the current position
                craneState.motorTicks   = 0;
                craneState.armTicks     = armStartPosition;
                craneState.plungerTicks = plungerStartPosition;

                // then set it to action state
                applicationState = actionState;
            }
            break;
        }
    }
}

// interrupt handling for the serial connection
ISR(serialInterrupt)
{
    if(serialData != '\r' && serialData != '\n' && serialData != '\0' && (serialInputData.index < 127))
    {
        // add to array
        serialInputData.buffer[serialInputData.index]     = serialData;
        serialInputData.buffer[serialInputData.index + 1] = '\0';

        serialInputData.index++;
    }
    else
    {
        // set update flag
        serialInputData.readFlag = 1;
        // reset message index
        serialInputData.index    = 0;
    }
}

// interrupt handling for the bluetooth connection
ISR(bluetoothInterrupt)
{
    if(bluetoothData != '\r' && bluetoothData != '\n' && bluetoothData != '\0' && (bluetoothInputData.index < 127))
    {
        // add to array
        bluetoothInputData.buffer[bluetoothInputData.index]     = bluetoothData;
        bluetoothInputData.buffer[bluetoothInputData.index + 1] = '\0';

        bluetoothInputData.index++;
    }
    else
    {
        // set update flag
        bluetoothInputData.readFlag = 1;
        // reset message index
        bluetoothInputData.index    = 0;
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
    PORTA = 0xff;

    DDRL  = 0xff;
    PORTL = 0x00;
}

void CRANE_getMovesFromEeprom(uint16_t addr)
{
    uint16_t address = addr;

    for(uint8_t i = 0; i < recordLength; i++)
    {
        uint16_t motorTicks = 0;

        // read the motor position
        motorTicks = CRANE_eepromReadChar(address++) << 8;
        motorTicks += CRANE_eepromReadChar(address++);

        recordedMoves[i].motorTicks = motorTicks;

        // read the arm position
        recordedMoves[i].armTicks = CRANE_eepromReadChar(address++);

        // read the plunger position
        recordedMoves[i].plungerTicks = CRANE_eepromReadChar(address++);
    }
}

void CRANE_saveMovesToEeprom(uint16_t addr)
{
    uint16_t address = addr;

    for(uint8_t i = 0; i < recordLength; i++)
    {
        // cheat by directly moving into our recorded array
        recordedMoves[i] = currentMoves[i];

        // save the motor position
        CRANE_eepromWriteChar((currentMoves[i].motorTicks & 0xff00) >> 8, address++);
        CRANE_eepromWriteChar((currentMoves[i].motorTicks & 0x00ff), address++);

        // save the arm position
        CRANE_eepromWriteChar(currentMoves[i].armTicks, address++);

        // save the plunger position
        CRANE_eepromWriteChar(currentMoves[i].plungerTicks, address++);
    }
}

uint8_t doesBufferMatch(volatile connectionBuffer_t buf, uint8_t strictMode, char const * const pStr)
{
    if(buf.readFlag)
    {
        // reset read flag
        buf.readFlag = strictMode ? 0 : buf.readFlag;

        return stringCompare(buf.buffer, pStr);
    }

    return 0;
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