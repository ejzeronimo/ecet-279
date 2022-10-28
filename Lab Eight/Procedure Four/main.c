/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 10/28/2022 4:32:43 PM
 * Author: Ethan Zeronik
 *
 * Operations: Mode enabled combination of previous parts
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTA               LED bar
 *   PORTF.0             Potentiometer in
 *   PORTD1              Direction encoder pin
 *   PORTD2              Encoder clock pin
 *   PORTD2              Mode pushbutton
 */

/* NOTE: Includes */
#include <avr/io.h>

#include "Encoder.h"
#include "AnalogToDigital.h"
#include "Delay.h"

/* NOTE: Custom Macros */
// TODO: None

/* NOTE: Global Variables */
static uint8_t modeFlag = 0;

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();
    
    ENC_init();
    ADC_initInterrupt();
    DLY_initInterrupt();

    sei();

    while(1)
    {
        // the watchdog led
        if(DLY_getTick() > 500)
        {
            PORTB = ~PORTB & 0x80;

            if(modeFlag == 0)
            {
                PORTA = ~PORTA & 0x01;
            }

            DLY_setTick(0);
        }

        // the mode switcher
        if(modeFlag == 1)
        {
            // get the value and bit shift it right 2
            PORTA = ADC_getTenBitValueInterrupt(0) >> 2;
        }
        else if(modeFlag == 2)
        {
            // get the flagged byte
            uint8_t directionFlag = ENC_getDirection();

            // if the flag has been set
            if(directionFlag & 0x10)
            {
                // get the driection flag
                if((directionFlag & 0x01))
                {
                    // move the bar left one
                    PORTA = PORTA << 1 | 0x01;
                }
                else
                {
                    // move the bar right one
                    PORTA = PORTA >> 1;
                }
            }
        }
    }
}

// the mode button interupt
ISR(INT3_vect)
{
    if(modeFlag < 2)
    {
        modeFlag++;
    }
    else
    {
        modeFlag = 0;
    }

    PORTA = 0x00;
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // set port B.7 as an output
    DDRB  = 0x80;
    PORTB = 0x00;

    // set port A.7 as an output
    DDRA  = 0xFF;
    PORTA = 0x00;

    // set port D.3 as an input
    DDRD  = 0x00;
    PORTD = 0x08;

    // set interrupt 3 to on rising edge
    EIMSK = (1 << INT3);
    EICRA = (1 << ISC31);
}
