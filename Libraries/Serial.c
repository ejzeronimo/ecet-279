/*
 * FileName: Delay.c
 * Version: 1
 *
 * Created: 11/2/2022 1:41 PM
 * Author: Ethan Zeronik
 *
 * Operations: serial implementation
 */

/* NOTE: Includes */
#include "Serial.h"

#include <avr/io.h>
#include <string.h>

/* NOTE: Local declarations */
// TODO: None

/* NOTE: Global Variables */
static asyncGetHandler_t interruptCallback;

/* NOTE: Local function implementations */
void SERIAL_uartInit(void)
{
    uint16_t baudCalc = ((16000000 / 9600) / 16) - 1;

    UCSR0A = 0x00;
    // enable UART TX and RX
    UCSR0B = 0x18;
    // set the UART for N, 8, 1
    UCSR0C = 0x06;
    // set BAUD Rate for 9600 with 16MHz clock
    UBRR0L = baudCalc;
    UBRR0H = 0;
}

void SERIAL_uartInitAsync(void)
{
    uint16_t baudCalc = ((16000000 / 9600) / 16) - 1;

    UCSR0A = 0x00;
    // enable UART TX and RX with interrupt flag
    UCSR0B = 0x98;
    // set the UART for N, 8, 1
    UCSR0C = 0x06;

    // set BAUD Rate for 9600 with 16MHz clock
    UBRR0L = baudCalc;
    UBRR0H = 0;
}

void SERIAL_uartSend(char const * const pTransmitString)
{
    for(size_t i = 0; i < strlen(pTransmitString); i++)
    {
        // wait for uart tx to be ready
        while((UCSR0A & (1 << UDRE0)) == 0)
        {
        }

        // send out uart
        UDR0 = pTransmitString[i];
    }
}

char SERIAL_uartGetSync(void)
{
    char ch;

    // wait for uart rx to be ready
    while((UCSR0A & (1 << RXC0)) == 0)
    {
    }

    // save to the char
    ch = UDR0;

    return ch;
}

void SERIAL_uartAsyncGetHandler(asyncGetHandler_t cb)
{
    // set the interal callback pointer to the one we were given
    interruptCallback = cb;
}

ISR(USART0_RX_vect)
{
    interruptCallback(UDR0);
}