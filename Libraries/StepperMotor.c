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

// TODO: move this
#define F_CPU 16000000UL
#include <util/delay.h>

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
    *pRegister = (*pRegister & 0xf0) | 0x0f;

    // turn on pullup resisitors on the bottom nibble
    *pPort = 0x00;

    // save the port pointer to the static var
    sMotorPort = pPort;
}

void SM_move(StepperMotorRunMode_t mode, double distance)
{
    uint8_t * pArray = NULL;
    uint8_t   size   = 0;
    uint32_t  steps  = 0;

    switch(mode)
    {
        case Wave:
        {
            pArray = sWaveStepMap;
            size   = sizeof(sWaveStepMap) / sizeof(uint8_t);
            steps  = (distance * 2048);
        }
        break;
        case Full:
        {
            pArray = sFullStepMap;
            size   = sizeof(sFullStepMap) / sizeof(uint8_t);
            steps  = (distance * 2048);
        }
        break;
        case Half:
        {
            pArray = sHalfStepMap;
            size   = sizeof(sHalfStepMap) / sizeof(uint8_t);
            steps  = (distance * 4096);
        }
        break;
        default:
            break;
    }

    for(uint32_t i = 0; i < steps; i++)
    {
        *sMotorPort = pArray[i % size];
        _delay_ms(3);
    }

    *sMotorPort = 0x00;
}

void SM_movePosition(StepperMotorRunMode_t mode, uint16_t distance)
{
    SM_move(mode, ((double)distance / 360));
}