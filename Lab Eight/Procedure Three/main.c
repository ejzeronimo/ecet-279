/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 10/26/2022 2:46:31 PM
 * Author: Ethan Zeronik
 *
 * Operations: Encoder module
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTA               LED bar
 *   PORTD1              Direction encoder pin
 *   PORTD2              Encoder clock pin
 */

/* NOTE: Includes */
#include <avr/io.h>

#include "Encoder.h"

/* NOTE: Custom Macros */
// TODO: None

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    ENC_init();

    IO_init();

    sei();

    while(1)
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

/* NOTE: Function implementations */
void IO_init(void)
{
    // set portA as an output
    DDRA  = 0xFF;
    PORTA = 0x00;
}