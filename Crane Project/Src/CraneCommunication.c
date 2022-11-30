/*
 * CraneCommunication.c
 *
 * Created: 2022/11/5 10:03:43 am
 * Author: xiang82, Alex Weyer
 */

/* NOTE: Includes */
#include "CraneCommunication.h"

#if !defined(F_CPU)
    #define F_CPU 16000000UL
#endif

/* NOTE: Local declarations */
// Xiangs's serial send function
void UART_out(uint8_t ch);
// Alex and Yu-Hung's serial send function
// transmit single byte of data
void BLUETOOTH_out(uint8_t ch);

/* NOTE: Global function implementations */
void CRANE_initSerial(uint16_t baudRate)
{
    // ubrr load
    uint16_t myubr;

    // set up the ucsr0a and ucsr0b and ucsr0c
    UCSR0A = 0x00;
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

    myubr  = (F_CPU / (16UL * (uint16_t)baudRate)) - 1;
    // load ubrr low
    UBRR0L = myubr;

    UBRR0H = 0x00;
}

void CRANE_sendSerial(char const * const pData)
{
    char const * pWorker = (char const *)pData;

    // while we are not at the end of the string
    while(*pWorker != '\0')
    {
        // wait for uart tx to be ready then send out uart
        UART_out(*pWorker);

        pWorker++;
    }
}

void CRANE_initBluetooth(uint16_t baudRate)
{
    uint16_t mybur;
    UCSR1A = 0;

    // enable receive interrupt
    // enable transmits
    // enable receive
    // 2 stop bits
    UCSR1B = (1 << RXCIE1) | (1 << TXEN1) | (1 << RXEN1);

    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);

    // set up baud rate
    mybur = (F_CPU) / (16UL * (uint16_t)baudRate) - 1;

    UBRR1L = mybur;
    UBRR1H = 0x00;
}

void CRANE_sendBluetooth(char const * const pData)
{
    char const * pWorker = (char const *)pData;

    // while we are not at the end of the string
    while(*pWorker != '\0')
    {
        // wait for uart tx to be ready then send out uart
        BLUETOOTH_out(*pWorker);

        pWorker++;
    }
}

/* NOTE: Local function implementations */
void UART_out(uint8_t ch)
{
    // wait to complete transmission and empty udr0
    while((UCSR0A & (1 << UDRE0)) == 0)
    {
    }

    // load next byte to be transmitted
    UDR0 = ch;
}

void BLUETOOTH_out(uint8_t ch) // transmit single byte of data
{
    while((UCSR1A & (1 << UDRE1)) == 0)
    {
        // wait for completing transmission and empty UDR0
    }

    UDR1 = ch; // load next byte to be transmitted
}