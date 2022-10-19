/*
 * FileName: AnalogToDigital.h
 * Version: 1
 *
 * Created: 10/19/2022 12:47 AM
 * Author: Ethan Zeronik
 *
 * Operations: header for the adc submodule
 */

#ifndef AnalogToDigital_h_INCLUDED
#define AnalogToDigital_h_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include "stdio.h"

#define F_CPU 16000000UL

/* NOTE: Custom Types */
// TODO: None

/* NOTE: Function prototypes */
// init registers for adc
void ADC_init(void);
// returns the value of the given channel
double ADC_getTenBitValue(uint16_t channel);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // AnalogToDigital_h_INCLUDED