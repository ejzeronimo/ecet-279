/*
 * CraneDelay.h
 *
 * Created: 10/26/2022 8:47:13 AM
 * Author: weyer4
 */

#ifndef CraneDelay_H_INCLUDED
#define CraneDelay_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include <avr/interrupt.h>
#include <avr/io.h>

// starts timer 0 in async mode
void CRANE_initTimer(void);
// returns the current delay in ms
uint16_t CRANE_tick(void);
// delays for the desired amount of ms
void CRANE_delayMs(uint16_t ms);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* CraneDelay_H_INCLUDED */