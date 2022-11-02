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
    // enable UART TX and RX
    UCSR0B = 0x98;
    // set the UART for N, 8, 1
    UCSR0C = 0x06;

    // set BAUD Rate for 9600 with 16MHz clock
    UBRR0L = baudCalc;
    UBRR0H = 0;
}

void SERIAL_uartSend(char const * const pTransmitString)
{
    char const * localPointer = pTransmitString;

    while(*localPointer != 0x00)
    {
        // wait for uart tx to be ready
        while((UCSR0A & (1 << UDRE0)) == 0)
        {
        }

        // send out uart
        UDR0 = *localPointer++;
    }
}

char SERIAL_uartGetSync(void)
{
    char ch;

    // wait for uart rx to be ready
    while((UCSR0A & (1 << RXC0)) == 0)
    {
    }

    ch = UDR0;

    return ch;
}

void SERIAL_uartAsyncGetHandler(asyncGetHandler_t cb)
{
    interruptCallback = cb;
}

ISR(USART0_RX_vect)
{
    interruptCallback(UDR0);
}