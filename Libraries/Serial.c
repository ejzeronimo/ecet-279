/*
 * FileName: Serial.c
 * Version: 1
 *
 * Created: 11/2/2022 1:41 PM
 * Author: Ethan Zeronik
 *
 * Operations: serial implementation
 */

/* NOTE: Includes */
#include "Serial.h"

#if !defined(F_CPU)
    #define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

/* NOTE: Local declarations */
// a helper to poll for the given registers then set the given value
// register and mask are related
// set will be set to the value at read once polling finishes
void pollThenSetHelper(uint8_t volatile * const pRegister, uint8_t mask, uint8_t volatile * const pSet, uint8_t volatile * const pRead);
// a helper to remove redundant logic for sending data
void sendCharHelper(SerialPortSelector_t port, char const * const value);

/* NOTE: Global Variables */
// the handlers for each main uart channel
static SerialAsyncGetHandler_t interruptCallback[4];

/* NOTE: Global function implementations */
void SERIAL_uartInit(SerialPortSelector_t port, uint32_t baud)
{
    uint16_t baudCalc = ((F_CPU / baud) / 16) - 1;

    switch(port)
    {
        default:
        case serialUsart0:
        {
            UCSR0A = 0x00;
            // enable UART TX and RX with interrupt flag
            UCSR0B = 0x18;
            // set the UART for N, 8, 1
            UCSR0C = 0x06;
            // set BAUD Rate for 16MHz clock
            UBRR0L = baudCalc;
            UBRR0H = (baudCalc >> 8) & 0x0f;
        }
        break;
        case serialUsart1:
        {
            UCSR1A = 0x00;
            UCSR1B = 0x18;
            UCSR1C = 0x06;
            UBRR1L = baudCalc;
            UBRR1H = (baudCalc >> 8) & 0x0f;
        }
        break;
        case serialUsart2:
        {
            UCSR2A = 0x00;
            UCSR2B = 0x18;
            UCSR2C = 0x06;
            UBRR2L = baudCalc;
            UBRR2H = (baudCalc >> 8) & 0x0f;
        }
        break;
        case serialUsart3:
        {
            UCSR3A = 0x00;
            UCSR3B = 0x18;
            UCSR3C = 0x06;
            UBRR3L = baudCalc;
            UBRR3H = (baudCalc >> 8) & 0x0f;
        }
        break;
    }
}

void SERIAL_uartInitAsync(SerialPortSelector_t port, uint32_t baud)
{
    SERIAL_uartInit(port, baud);

    // turn on the rx interrupt
    switch(port)
    {
        default:
        case serialUsart0:
            UCSR0B = 0x98;
            break;
        case serialUsart1:
            UCSR1B = 0x98;
            break;
        case serialUsart2:
            UCSR2B = 0x98;
            break;
        case serialUsart3:
            UCSR3B = 0x98;
            break;
    }
}

void SERIAL_uartSend(SerialPortSelector_t port, char const * const pTransmitString)
{
    char const * pWorker = (char const *)pTransmitString;

    // while we are not at the end of the string
    while(*pWorker != '\0')
    {
        // wait for uart tx to be ready then send out uart
        sendCharHelper(port, pWorker);

        pWorker++;
    }
}

void SERIAL_uartSendFixed(SerialPortSelector_t port, char const * const pTransmitString, uint16_t length)
{
    for(uint16_t i = 0; i < length; i++)
    {
        // wait for uart tx to be ready then send out uart
        sendCharHelper(port, pTransmitString + i);
    }
}

char SERIAL_uartGetSync(SerialPortSelector_t port)
{
    char ch;

    // wait for uart rx to be ready and save to the char
    switch(port)
    {
        default:
        case serialUsart0:
            pollThenSetHelper(&UCSR0A, (1 << RXC0), (uint8_t volatile * const)&ch, &UDR0);
            break;
        case serialUsart1:
            pollThenSetHelper(&UCSR1A, (1 << RXC1), (uint8_t volatile * const)&ch, &UDR1);
            break;
        case serialUsart2:
            pollThenSetHelper(&UCSR2A, (1 << RXC2), (uint8_t volatile * const)&ch, &UDR2);
            break;
        case serialUsart3:
            pollThenSetHelper(&UCSR3A, (1 << RXC3), (uint8_t volatile * const)&ch, &UDR3);
            break;
    }

    return ch;
}

void SERIAL_uartAsyncGetHandler(SerialPortSelector_t const port, SerialAsyncGetHandler_t cb)
{
    // set the interal callback pointer to the one we were given
    interruptCallback[port] = cb;
}

/* NOTE: Local function implementations */
void pollThenSetHelper(uint8_t volatile * const pRegister, uint8_t mask, uint8_t volatile * const pSet, uint8_t volatile * const pRead)
{
    // wait for the register
    while((*pRegister & mask) == 0)
    {
    }

    // save to the pSet
    *pSet = *pRead;
}

void sendCharHelper(SerialPortSelector_t port, char const * const value)
{
    switch(port)
    {
        default:
        case serialUsart0:
            pollThenSetHelper(&UCSR0A, (1 << UDRE0), &UDR0, (uint8_t volatile * const)value);
            break;
        case serialUsart1:
            pollThenSetHelper(&UCSR1A, (1 << UDRE1), &UDR1, (uint8_t volatile * const)value);
            break;
        case serialUsart2:
            pollThenSetHelper(&UCSR2A, (1 << UDRE2), &UDR2, (uint8_t volatile * const)value);
            break;
        case serialUsart3:
            pollThenSetHelper(&UCSR3A, (1 << UDRE3), &UDR3, (uint8_t volatile * const)value);
            break;
    }
}

ISR(USART0_RX_vect)
{
    interruptCallback[serialUsart0](UDR0);
}

ISR(USART1_RX_vect)
{
    interruptCallback[serialUsart1](UDR1);
}

ISR(USART2_RX_vect)
{
    interruptCallback[serialUsart2](UDR2);
}

ISR(USART3_RX_vect)
{
    interruptCallback[serialUsart3](UDR3);
}