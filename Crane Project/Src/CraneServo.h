/*
 * CraneServo.h
 *
 * Created: 2022/11/17 下午 09:01:37
 *  Author: User
 */ 


#ifndef CraneServo_H_INCLUDED
#define CraneServo_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include <avr/io.h>

// should set up the timer
void CRANE_initServos(void);
// should start the pwm signal back at the previous position
void CRANE_startServos(void);
// should stop the pwm signal and save the position
void CRANE_stopServos(void);
// should change the position of the servo
// if servo = 0 then output compare 0 changes
// if servo = 1 then output comapre 1 changes
// 1ms = 0 and 2ms = 255
void CRANE_setServoPosition(uint8_t servo, uint8_t position);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* CraneServo_H_INCLUDED */