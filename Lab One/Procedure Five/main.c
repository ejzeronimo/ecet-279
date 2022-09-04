/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 8/31/2022 2:22:03 PM
 * Author: Ethan Zeronik
 *
 * Operations: sweeps an led bar from right to left
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTB               buttons for the sweep control
 *   PORTA               LED bar (all 8 pins used)
 */

#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

/* NOTE: Custom Macros */
// pin for the start button
#define Start   00
// pin for the pause button
#define Pause   01
// pin for the reset button
#define Reset   02
// macro to get the current pins
#define Buttons (PINB & ((1 << Start) | (1 << Pause) | (1 << Reset)))

/* NOTE: Global Variables */
static int8_t sweepFlag;

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);
// sweeps the given port's bits from 0x00 to 0xff then back to 0x00
void LED_sweep(volatile uint8_t * port);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    uint8_t inputSwitches;

    IO_init();

    while(1)
    {
        inputSwitches = Buttons;

        // play button was pressed
        if(!(inputSwitches & (1 << Start)))
        {
            // set the flag so that the sweep function has a direction
            sweepFlag = 1;
        }
        // reset button was pressed
        else if(!(inputSwitches & (1 << Reset)))
        {
            // turn off the leds and reset the sweep direction
            sweepFlag = 0;
            PORTA     = 0x00;
        }
        // if no pause button and the sweep has a direction
        else if((inputSwitches & (1 << Pause)) && sweepFlag)
        {
            // DEBUG: comment this line out when using simulator
            _delay_ms(100);

            LED_sweep(&PORTA);
        }
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // set port b as input because it has buttons on it
    DDRB  = 0x00;
    // turn on pullup resisitors
    PORTB = 0xFF;

    // set all 8 leds as outputs
    DDRA  = 0xFF;
    // turn off all leds
    PORTA = 0x00;
}

void LED_sweep(volatile uint8_t * port)
{
    // moving up the line
    if(sweepFlag == 1)
    {
        // if the last led is on
        if(*port & 0x80)
        {
            sweepFlag = -1;
        }

        // move over leds by one
        *port = *port << 1;
        // then add a new led light at LSB
        *port = *port | 0x01;
    }

    // moving down the line
    if(sweepFlag == -1)
    {
        // if all the leds are off
        if(*port == 0x00)
        {
            sweepFlag = 1;
        }

        // move over leds by one
        *port = *port >> 1;
    }
}
