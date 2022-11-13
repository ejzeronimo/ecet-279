/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 11/2/2022 1:39:55 PM
 * Author: Ethan Zeronik
 *
 * Operations: basic serial
 *
 * Hardware:
 *   Atmega2560          micro controller
 *   PORTA               LED bar
 *   PORTC               Button/Switches
 */

/* NOTE: Includes */
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Serial.h"

/* NOTE: Custom Macros */
// TODO: None

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);
// handler
void asyncGetHandler(char c);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();

    // init async uart and bind an interrupt handler
    SERIAL_uartInitAsync(serialUsart0, 9600);
    SERIAL_uartAsyncGetHandler(serialUsart0, &asyncGetHandler);

    sei();

    while(1)
    {
        SERIAL_uartSendFixed(serialUsart0, (char const * const)&PINC, 1);
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // set portA as an output
    DDRA  = 0xFF;
    PORTA = 0x00;

    // set portC as an input
    DDRC  = 0x00;
    PORTC = 0xFF;
}

void asyncGetHandler(char c)
{
    PORTA = c;
}
