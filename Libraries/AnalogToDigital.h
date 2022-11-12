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

#include <avr/interrupt.h>
#include <stdio.h>

#define F_CPU 16000000UL

/* NOTE: Custom Types */
// typing for the handler function
typedef void (*AnalogAsyncGetHandler_t)(uint16_t);

/* NOTE: Function prototypes */
// init registers for adc
void   ADC_init(void);
// init adc for interrupt mode
void   ADC_initInterrupt(void);
// returns the value of the given channel
double ADC_getTenBitValue(uint16_t channel);
// gets the 10 bit value on the channel
uint16_t ADC_getTenBitValueInterrupt(uint16_t channel);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // AnalogToDigital_h_INCLUDED