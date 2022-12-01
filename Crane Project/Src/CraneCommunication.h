/*
 * CraneCommunication.h
 *
 * Created: 11/2/2022 8:32:52 AM
 *  Author: xiang82, Alex Weyer, Yu-Hung (Thomas) Wang
 */ 

#ifndef CraneCommunication_H_INCLUDED
#define CraneCommunication_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// defines in case of no callback
#define serialInterrupt USART0_RX_vect
#define bluetoothInterrupt USART1_RX_vect

// defines for use in interrupt
#define serialData UDR0
#define bluetoothData UDR1

// sets up usart0 for serial communication 
void CRANE_initSerial(uint16_t baudRate);
// sends the given string to the main serialport
void CRANE_sendSerial(char const * const pData);
// sets up usart1 for serial communication
void CRANE_initBluetooth(uint16_t baudRate);
// sends the given string to the bluetooth serialport
void CRANE_sendBluetooth(char const * const pData);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* CraneCommunication_H_INCLUDED */