/*
 * FileName: Encoder.h
 * Version: 1
 *
 * Created: 10/26/2022 2:28 PM
 * Author: Ethan Zeronik
 *
 * Operations: encoder definition
 */

#ifndef Encoder_h_INCLUDED
#define Encoder_h_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#pragma message("WARNING: this module uses PORTD1:2 for interrupt")

#include <avr/interrupt.h>
#include <stdio.h>

/* NOTE: Custom Types */
// TODO: None

/* NOTE: Function prototypes */
// init for the encoder
void ENC_init(void);
// gets the raw value for the port
uint8_t ENC_getValue(void);
// returns the flagged direction
uint8_t ENC_getDirection(void);


#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // Encoder_h_INCLUDED