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

#include <stdint.h>

/* NOTE: Function prototypes */
// init registers for delay
void     DLY_init(void);
// init for the 1ms interrupt service
void     DLY_initInterrupt(void);
// delay for an amount of ms
void     DLY_ms(double ms);
// gets the ISR tick value
uint16_t DLY_getTick(void);
// sets the ISR tick value
void     DLY_setTick(uint16_t t);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // Delay_h_INCLUDED