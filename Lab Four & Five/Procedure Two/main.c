/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 9/19/2022 8:49:04 AM
 * Author: Ethan Zeronik
 *
 * Operations: creation of application structure and testing
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTx.x             LED13 active high
 */

#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "Debugger.h"

/* NOTE: Custom Macros */
#define startButton (PINK & 0x01)

#define hotButton  (PINA & 0x10)
#define warmButton (PINA & 0x20)
#define coldButton (PINA & 0x40)
#define doorSwitch (PINA & 0x80)

#define outPort (PORTC)

#define doneLed(S)    ((PORTC & ~0x01) | (S << 0))
#define agitateLed(S) ((PORTC & ~0x02) | (S << 1))
#define spinLed(S)    ((PORTC & ~0x04) | (S << 2))

#define drainValve(S) ((PORTC & ~0x10) | (S << 4))
#define hotValve(S)   ((PORTC & ~0x20) | (S << 5))
#define coldValve(S)  ((PORTC & ~0x40) | (S << 6))

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);
// returns what valves need to be opened
void WASH_setValvesToInput(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    initDebug();

    IO_init();

    while(1)
    {
        // while we have not started the washing machine
        // and the door is open
        while(!((startButton == 0x01) && (doorSwitch == 0x00)))
        {
            // do nothing
        }

        // NOTE: fill cycle
        WASH_setValvesToInput();

        _delay_ms(4000);

        outPort = hotValve(0);
        outPort = coldValve(0);

        // NOTE: wash cycle
        outPort = agitateLed(1);

        for(size_t i = 0; i < 2; i++)
        {
            // move cw for 2 seconds
            _delay_ms(2000);

            // move ccw for 2 seconds
            _delay_ms(2000);
        }

        outPort = agitateLed(0);

        // NOTE: drain cycle
        outPort = drainValve(1);

        _delay_ms(4000);

        outPort = drainValve(0);

        // NOTE: fill again cycle
        WASH_setValvesToInput();

        _delay_ms(4000);

        outPort = hotValve(0);
        outPort = coldValve(0);

        // NOTE: rinse cycle
        // agitate 12 seconds

        // NOTE: rinse again cycle
        outPort = drainValve(1);

        // wait 15 s
        // spin for 9s

        outPort = drainValve(0);

        // NOTE: done with the wash
        outPort = doneLed(1);

        while(doorSwitch != 0x80)
        {
            // do nothing
        }

        outPort = doneLed(0);
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // bottom nibble is motor and top is input switches
    DDRA  = 0x0f;
    // turn on switch pullup resistors
    PORTA = 0xf0;

    // the start button
    DDRK  = 0x00;
    PORTK = 0x01;

    // the led output port
    DDRC  = 0xff;
    PORTC = 0x00;
}

void WASH_setValvesToInput(void)
{
    if(hotButton != 0x00)
    {
        // hot on, cold off
        outPort = hotValve(1);
        outPort = coldValve(0);
    }

    if(warmButton != 0x00)
    {
        // hot on, cold on
        outPort = hotValve(1);
        outPort = coldValve(1);
    }

    if(coldButton != 0x00)
    {
        // hot off, cold on
        outPort = hotValve(0);
        outPort = coldValve(1);
    }
}