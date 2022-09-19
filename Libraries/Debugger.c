/*
File Name: Debugger.c
Create By: Nicholas Heinz
Create Date: 4/30/2022

Version: 1.2
Last Modified: 8/27/2022
Last Modified By: Nicholas Heinz

Change Log:
	Version 1.2 (Nicholas Heinz - 8/27/2022)
	-	Removed turning on PORTA during transmission, was originally used for
		timing how long data transmission takes.
	-	Added DDRD, PORTD, and PIND
	
	Version 1.1 (Nicholas Heinz - 5/4/2022)
	-	Removed sending register name before sending register.
	
	Version 1.0 (Nicholas Heinz - 4/30/2022)
	-	Created
*/

#include "Debugger.h"

// Start/Stop Messages
char st[] = {"ST"};
char en[] = {"END"};

// Port Addresses and Order
volatile uint8_t *portAddr[] = {&DDRA, &PORTA, &PINA,
						&DDRC, &PORTC, &PINC,
						&DDRL, &PORTL, &PINL,
						&DDRD, &PORTD, &PIND,
						&DDRF, &PORTF, &PINF,
						&DDRK, &PORTK, &PINK};

// Timer Addresses and Order					
volatile uint8_t *timerAddr[] = {&TCCR0A, &TCCR0B, &TIMSK0, &OCR0A, &OCR0B,
						&TCCR1A, &TCCR1B, &TIMSK1, &OCR1AL, &OCR1BL, &OCR1CL,
						&TCCR2A, &TCCR2B, &TIMSK2, &OCR2A, &OCR2B,
						&TCCR3A, &TCCR3B, &TIMSK3, &OCR3AL, &OCR3BL, &OCR3CL};

// USART Addresses and Order					
volatile uint8_t *USARTAddr[] = {&UCSR0A, &UCSR0B, &UCSR0C,
						&UCSR1A, &UCSR1B, &UCSR1C};

// ADC Addresses and Order						
volatile uint8_t *ADCAddr[] = {&ADCSRA, &ADCSRB, &ADMUX};

// Interrupt Addresses and Order
volatile uint8_t *intAddr[] = {&EICRA, &EICRB, &EIMSK};

/*
Function: initDebug
Inputs: None
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Configures the USART registers for data transmission, Configures the
	Timer registers, and enables interrupts.
*/
void initDebug()
{
	initDebugUSART();
	initDebugTimer();
	sei();
}

/*
Function: initDebugTimer
Inputs: None
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Uses Timer5 with a prescaler of 256, and enables overflow interrupt
*/
void initDebugTimer()
{
	//Prescaler 256
	TCCR5B |= (1<<CS52) | (1<<CS50);
	
	//15625 TOP Value for UPDATE_FREQUENCY
	TCNT5 = 65535 - (F_CPU / (1024*UPDATE_FREQUENCY));
	
	//Enable Timer Overflow Interrupt
	TIMSK5 = (1<<TOIE5);
}

/*
Function: ISR Timer 5 Overflow
Inputs: None
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Resets counter value then sends register data
*/
ISR(TIMER5_OVF_vect) 
{
	TCNT5 = 65535 - (F_CPU / (1024*UPDATE_FREQUENCY));
	sendRegisterData();
}

/*
Function: initDebugUSART
Inputs: None
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Enables USART0 (Connected to USB)
	8-bits, 25000 Baud Rate
*/
void initDebugUSART()
{
	//Enable RX and TX
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	
	//Set Communication Modes
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);	//8 Data bits
	UBRR0 = 3;							//250000 Baud Rate	
}

/*
Function: UART0Out
Inputs: uint8_t data - value to be sent
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Sends 8-bit unsigned integer out UART0
*/
void UART0Out(uint8_t data) 
{
	//Wait for previous transmission to be done
	while(!(UCSR0A & (1<<UDRE0)));
	
	//Set register for next transmission
	UDR0 = data;	
}

/*
Function: sendStopBit
Inputs: None
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Sends 0x0A to signify stop
*/
void sendStopBit()
{
	//Wait for previous transmission to be done
	while(!(UCSR0A & (1<<UDRE0)));
	
	//Set register for next transmission
	UDR0 = 0x0A;
}

/*
Function: UART0PrintString
Inputs: char *strPtr - Pointer to string array
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Continues to send data until reaching null character
*/
void UART0PrintString(char *strPtr)
{
	while(*strPtr != 0x00) 
	{
		UART0Out(*strPtr);
		strPtr ++;
	}
}

/*
Function: sendPortData
Inputs: None
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Loops through portAddr array and sends
	out data at each address
*/
void sendPortData()
{	
	for (uint8_t i=0; i<18; i++)
	{
		UART0Out(*portAddr[i]);
	}
}

/*
Function: sendTimerData
Inputs: None
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Loops though timerAddr array and sends
	out data at each address
*/
void sendTimerData()
{
	//Timer 0
	for (uint8_t i=0; i<3; i++) 
	{
		UART0Out(*timerAddr[i]);
	}
	
	//OCR0A
	UART0Out(*timerAddr[3]);
	UART0Out(*(timerAddr[3]+1));
	
	//OCR0B
	UART0Out(*timerAddr[4]);
	UART0Out(*(timerAddr[4]+1));
	
	//Timer 1
	for (uint8_t i=5; i<8; i++)
	{
		UART0Out(*timerAddr[i]);
	}
	
	//OCR1A
	UART0Out(*timerAddr[8]);
	UART0Out(*(timerAddr[8]+1));
	
	//OCR1B
	UART0Out(*timerAddr[9]);
	UART0Out(*(timerAddr[9]+1));
	
	//OCR1C
	UART0Out(*timerAddr[10]);
	UART0Out(*(timerAddr[10]+1));
	
	//Timer 2
	for (uint8_t i=11; i<14; i++)
	{
		UART0Out(*timerAddr[i]);
	}
	
	//OCR2A
	UART0Out(*timerAddr[14]);
	UART0Out(*(timerAddr[14]+1));
	
	//OCR2B
	UART0Out(*timerAddr[15]);
	UART0Out(*(timerAddr[15]+1));
	
	//Timer 3
	for (uint8_t i=16; i<19; i++)
	{
		UART0Out(*timerAddr[i]);
	}
	
	//OCR3A
	UART0Out(*timerAddr[19]);
	UART0Out(*(timerAddr[19]+1));
	
	//OCR3B
	UART0Out(*timerAddr[20]);
	UART0Out(*(timerAddr[20]+1));
	
	//OCR3C
	UART0Out(*timerAddr[21]);
	UART0Out(*(timerAddr[21]+1));
}

/*
Function: sendUSARTData
Inputs: None
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Loops though USARTAddr array and sends
	out data at each address
*/
void sendUSARTData()
{
	for (uint8_t i=0; i<6; i++)
	{
		UART0Out(*USARTAddr[i]);
	}
}

/*
Function: sendADCData
Inputs: None
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Loops though ADCAddr array and sends
	out data at each address
*/
void sendADCData()
{
	for (uint8_t i=0; i<3; i++)
	{
		UART0Out(*ADCAddr[i]);
	}
}

/*
Function: sendINTData
Inputs: None
Outputs: None

Last Modified: 5/4/2022
Last Modified By: Nicholas Heinz

Description:
	Loops though intAddr array and sends
	out data at each address
*/
void sendINTData()
{
	for (uint8_t i=0; i<3; i++)
	{
		UART0Out(*intAddr[i]);
	}
}

/*
Function: sendRegisterData
Inputs: None
Outputs: None

Last Modified: 8/27/2022
Last Modified By: Nicholas Heinz

Description:
	Calls each send___Data() function
*/
void sendRegisterData() 
{
	UART0PrintString(st);
	sendStopBit();
	
	sendPortData();
	sendTimerData();
	sendUSARTData();
	sendADCData();
	sendINTData();
	
	UART0PrintString(en);
	sendStopBit();
}
