/*
 * FileName: StepperMotor.c
 * Version: 1
 *
 * Created: 9/14/2022 2:00 PM
 * Author: Ethan Zeronik
 *
 * Operations: run the stepper motor in one of three modes
 */

/* NOTE: Includes */
#include "StepperMotor.h"

// allows for variable delay
#define __DELAY_BACKWARD_COMPATIBLE__
#define F_CPU 16000000UL
#include <util/delay.h>

/* NOTE: Local declarations */
typedef struct StepperMotorModeData_t
{
    // size of the array
    size_t                arraySize;
    // pointer to the array
    uint8_t const * const pArray;
    // number of steps to take for desired rotation
    uint32_t              steps;
} StepperMotorModeData_t;

// returns the amount of steps needed for the given mode
// rotation is in radians (I think)
StepperMotorModeData_t getModeAndSteps(StepperMotorRunMode_t mode, double rotation);

/* NOTE: Global Variables */
// implementation of the wave step map
static uint8_t sWaveStepMap[4] = {
    0x01,
    0x02,
    0x04,
    0x08,
};

// implementation of the full step map
static uint8_t sFullStepMap[4] = {
    0x03,
    0x06,
    0x0c,
    0x09,
};

// implementation of the wave step map
static uint8_t sHalfStepMap[8] = {
    0x09,
    0x01,
    0x03,
    0x02,
    0x06,
    0x04,
    0x0c,
    0x08,
};

// instance pointer to the motor port
static volatile uint8_t * sMotorPort;

/* NOTE: Function implementations */
void SM_init(volatile uint8_t * pRegister, volatile uint8_t * pPort)
{
    // configure port register
    *pRegister |= 0x0f;

    // turn on pullup resisitors on the bottom nibble
    *pPort = (*pPort & 0xf0) | 0x00;

    // save the port pointer to the static var
    sMotorPort = pPort;
}

void SM_move(StepperMotorRunMode_t mode, double distance)
{
    StepperMotorModeData_t data = getModeAndSteps(mode, distance);

    for(uint32_t i = 0, j = 0; i < data.steps; i++)
    {
        *sMotorPort = (*sMotorPort & 0xf0) | data.pArray[j++];

        if(j >= data.arraySize)
        {
            j = 0;
        }

        _delay_ms(3);
    }

    *sMotorPort = *sMotorPort & 0xf0;
}

void SM_movePosition(StepperMotorRunMode_t mode, uint16_t distance)
{
    SM_move(mode, ((double)distance / 360));
}

void SM_moveTime(StepperMotorRunMode_t mode, bool direction, double time, double stepTime)
{
    StepperMotorModeData_t data = getModeAndSteps(mode, 0);

    for(uint32_t i = 0, j = (direction ? data.arraySize : 0); i < (time / stepTime); i++)
    {
        *sMotorPort = (*sMotorPort & 0xf0) | data.pArray[(direction ? j-- : j++)];

        if(j >= data.arraySize || j <= 0)
        {
            j = (direction ? data.arraySize : 0);
        }

        _delay_ms(stepTime);
    }

    *sMotorPort = *sMotorPort & 0xf0;
}

/* NOTE: Local function implementations */
StepperMotorModeData_t getModeAndSteps(StepperMotorRunMode_t mode, double rotation)
{
    uint8_t * pArray = NULL;
    uint8_t   size   = 0;
    uint32_t  steps  = 0;

    switch(mode)
    {
        case Wave:
        {
            pArray = sWaveStepMap;
            size   = sizeof(sWaveStepMap) / sizeof(sWaveStepMap[0]);
            steps  = (rotation * 2048);
        }
        break;
        case Full:
        {
            pArray = sFullStepMap;
            size   = sizeof(sFullStepMap) / sizeof(sFullStepMap[0]);
            steps  = (rotation * 2048);
        }
        break;
        case Half:
        {
            pArray = sHalfStepMap;
            size   = sizeof(sHalfStepMap) / sizeof(sHalfStepMap[0]);
            steps  = (rotation * 4096);
        }
        break;
        default:
            break;
    };

    return (StepperMotorModeData_t){
        .pArray    = pArray,
        .steps     = steps,
        .arraySize = size,
    };
}