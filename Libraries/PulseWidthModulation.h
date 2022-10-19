/*
 * FileName: PulseWidthModulation.h
 * Version: 1
 *
 * Created: 10/18/2022 10:14 PM
 * Author: Ethan Zeronik
 *
 * Operations: header for the pwm submodule
 */

#ifndef PulseWidthModulation_h_INCLUDED
#define PulseWidthModulation_h_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#pragma message("WARNING: this module uses Timer 1 for PWM")

#include "stdio.h"

#define F_CPU 16000000UL

/* NOTE: Custom Types */
// TODO: None

/* NOTE: Function prototypes */
// init registers for PWM
void PWM_init(void);
// makes a pwm for a given duty cycle
void PWM_dutyCycle(double percent);
// enable pwm output
void PWM_enable(void);
// disable pwm output
void PWM_disable(void);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // PulseWidthModulation_h_INCLUDED