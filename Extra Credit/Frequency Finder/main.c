/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 11/13/2022 2:38:17 PM
 * Author: Ethan Zeronik
 *
 * Operations: frequency finder for extra credit
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTL               LCD data
 *   PORTG.0             LCD RS
 *   PORTG.1             LCD R/W
 *   PORTG.2             LCD E
 */

/* NOTE: Includes */
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "LiquidCrystalDisplay.h"

/* NOTE: Custom Types */
// enum for state machine
typedef enum
{
    // the start first edge of the wave
    captureFirst,
    // the second edge of the wave
    captureSecond,
    // pause state, for printing and resetting
    captureDone
} captureState_t;

/* NOTE: Global Variables */
// done icon
LcdCustomCharacter_t check = {
    0b00000,
    0b00001,
    0b00001,
    0b00010,
    0b00010,
    0b00100,
    0b10100,
    0b01000,
};
// x icon
LcdCustomCharacter_t error = {
    0b00000,
    0b10001,
    0b10001,
    0b01010,
    0b00100,
    0b01010,
    0b10001,
    0b10001,
};
// rise edge icon
LcdCustomCharacter_t rise = {
    0b00000,
    0b00111,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b11100,
    0b00000,
};
// high side icon
LcdCustomCharacter_t high = {
    0b00000,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
};
// fall edge icon icon
LcdCustomCharacter_t fall = {
    0b00000,
    0b11100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00111,
    0b00000,
};
// low side icon icon
LcdCustomCharacter_t low = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b00000,
};
// state of the wave capture
volatile captureState_t state = captureDone;
// first time and second time that has been recorded, and the count of how
volatile uint16_t       firstTime, secondTime, overflowCountCached;
// the count of how many overflows there have been
volatile uint32_t       overflowCount;
// flag
volatile bool           overflowError;

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    // set the ICP pin as an input just in case
    DDRL  = 0x00;
    PORTL = 0x01;

    TCCR4A = 0x00;
    // set to capture on rising edge
    TCCR4B = (1 << ICES4);
    // turn on the input and overflow interrupts
    TIMSK4 |= (1 << ICIE4) | (1 << TOIE4);

    // turn on the LCD
    LCD_init(&DDRG, &PORTG, &DDRA, &PORTA);
    // add all the fancy characters
    LCD_createCharacter(lcdFirstSlot, check);
    LCD_createCharacter(lcdSecondSlot, error);
    LCD_createCharacter(lcdThirdSlot, rise);
    LCD_createCharacter(lcdFourthSlot, high);
    LCD_createCharacter(lcdFifthSlot, fall);
    LCD_createCharacter(lcdSixthSlot, low);
    LCD_createCharacter(lcdSeventhSlot, rise);
    // clear and home
    LCD_sendInstruction(0x01);
    LCD_sendInstruction(0x02);

    // start the timer
    TCCR4B |= (1 << CS40);
    // turn on the interrupts
    sei();

    while(1)
    {
        // start the capture
        state = captureFirst;

        // wait for the capture to be done
        while(state != captureDone)
        {
            // do nothing
        }

        LCD_sendInstruction(0x82);
        LCD_sendChar(LCD_getCharacter(lcdFirstSlot));

        if(overflowError)
        {
            // we cannot measure the frequency
            LCD_sendInstruction(0x82);
            LCD_sendChar(LCD_getCharacter(lcdSecondSlot));
        }
        else
        {
            // the the period and frequency
            uint32_t period = (secondTime + (overflowCountCached * 0x10000UL)) - firstTime;
            uint32_t freq   = F_CPU / period;

            // buffer for the bottom row of the lcd
            char lcdBuf[16];

            // add the XXXHz into the buffer
            uint8_t len = sprintf(lcdBuf, "%uhz", freq);

            // set the rest of the buffer to empty spaces
            for(uint8_t i = len; i < 16; i++)
            {
                lcdBuf[i] = ' ';
            }

            // move cursor and print
            LCD_sendInstruction(0xC0);
            LCD_sendString(lcdBuf);
        }

        // little animation
        for(uint8_t i = 0x02; i < 0x06; i++)
        {
            LCD_sendInstruction(0x80);
            LCD_sendChar(i);
            LCD_sendChar(i + 1);
            _delay_ms(200);
        }
    }
}

/* NOTE: Function implementations */
ISR(TIMER4_CAPT_vect)
{
    switch(state)
    {
        // we capture the first edge
        case captureFirst:
        {
            // store the relative time
            firstTime = ICR4;

            // reset the values
            overflowCount = 0;
            overflowError = false;
            state         = captureSecond;
        }
        break;
        // then capture the second edge
        case captureSecond:
        {
            // store the relatve time
            secondTime = ICR4;

            // set the state to done and overflow value
            overflowCountCached = overflowCount;
            state               = captureDone;
        }
        break;
        // get the compiler to stop complaining
        default:
            break;
    }
}

ISR(TIMER4_OVF_vect)
{
    // increment the overflow count
    overflowCount++;

    // of the overflow count has overflown, then there is an error
    overflowError = !overflowCount ? true : false;
}