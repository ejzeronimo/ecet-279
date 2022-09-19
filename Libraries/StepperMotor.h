/*
 * FileName: StepperMotor.h
 * Version: 1
 *
 * Created: 9/14/2022 2:00 PM
 * Author: Ethan Zeronik
 *
 * Operations: header for the stepper motor submobule
 */

#ifndef StepperMotor_h_INCLUDED
#define StepperMotor_h_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* NOTE: Custom Types */
// typing for the stepper motor enum
typedef enum StepperMotorRunMode_t
{
    // wave step mode
    Wave = 0,
    // wave step mode
    Full = 1,
    // wave step mode
    Half = 2,
} StepperMotorRunMode_t;

/* NOTE: Function prototypes */
// inits IO for the stepper motor
// takes a pointer to the port to use, assumes botom nibble
void SM_init(volatile uint8_t * pRegister, volatile uint8_t * pPort);

// moves the motor in the given mode to the given distance
// distance is in units of rotation
void SM_move(StepperMotorRunMode_t mode, double distance);

// moves the motor in the given mode to the given position
// distance is in units of degrees
void SM_movePosition(StepperMotorRunMode_t mode, uint16_t distance);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // StepperMotor_h_INCLUDED