/*
 * FileName: Delay.h
 * Version: 1
 *
 * Created: 10/18/2022 7:26 PM
 * Author: Ethan Zeronik
 *
 * Operations: header for the delay submodule
 */

#ifndef Delay_h_INCLUDED
#define Delay_h_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#pragma message("WARNING: this module uses Timer 0 for delays")

#include "stdio.h"

#define F_CPU 16000000UL

/* NOTE: Custom Types */
// TODO: None

/* NOTE: Function prototypes */
// init registers for delay
void DLY_init(void);
// delay for an amount of ms
void DLY_ms(double ms);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // Delay_h_INCLUDED