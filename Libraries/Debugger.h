/*
File Name: Debugger.h
Create By: Nicholas Heinz
Create Date: 4/30/2022

Version: 1.2
Last Modified: 8/27/2022
Last Modified By: Nicholas Heinz
*/


#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include <stdio.h>
#include <avr/interrupt.h>

#define UPDATE_FREQUENCY 10

#define F_CPU 16000000UL

void initDebug();

void initDebugTimer();
void initDebugUSART();
void sendRegisterData();

void sendPortData();
void sendTimerData();
void sendUSARTData();
void sendADCData();
void sendINTData();

void UART0Out(uint8_t);
void sendStopBit();

void UART0PrintString(char *strPtr);



#endif /* DEBUGGER_H_ */