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

#pragma message("WARNING: this module uses the bottom nibble of the provided port")

#include <stdbool.h>
#include <stdint.h>

/* NOTE: Custom Types */
// typing for the stepper motor enum
typedef enum StepperMotorRunMode_t
{
    // wave step mode
    stepperModeWave = 0,
    // full step mode
    stepperModeFull = 1,
    // half step mode
    stepperModeHalf = 2,
} StepperMotorRunMode_t;

/* NOTE: Function prototypes */
// inits IO for the stepper motor
// takes a pointer to the port to use, assumes botom nibble
void SM_init(uint8_t volatile * const pRegister, uint8_t volatile * const pPort);
// moves the motor in the given mode to the given distance
// distance is in units of rotation
void SM_move(StepperMotorRunMode_t mode, double distance);
// moves the motor in the given mode to the given position
// distance is in units of degrees
void SM_movePosition(StepperMotorRunMode_t mode, uint16_t distance);
// moves the motor in the given mode and the given direction for the given time
// 1 is CW and 0 is CCW
// both times are in ms
void SM_moveTime(StepperMotorRunMode_t mode, bool direction, double time, double stepTime);
// moves the motor in the given mode and the given direction for the given distance
// distance is in steps
// 1 is CW and 0 is CCW
void SM_moveStepsSigned(StepperMotorRunMode_t mode, bool direction, uint16_t distance);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // StepperMotor_h_INCLUDED