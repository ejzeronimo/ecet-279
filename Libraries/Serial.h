/*
 * FileName: Serial.h
 * Version: 1
 *
 * Created: 11/2/2022 1:41 PM
 * Author: Ethan Zeronik
 *
 * Operations: serial definition
 */

#ifndef Serial_h_INCLUDED
#define Serial_h_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#pragma message("WARNING: this module uses TX/RX0 for interrupts and serial communication")

#include <avr/interrupt.h>

/* NOTE: Custom Types */
typedef void (*asyncGetHandler_t)(char);

/* NOTE: Function prototypes */
// init for the serial sync mode
void SERIAL_uartInit(void);
// init for the serial async mode
void SERIAL_uartInitAsync(void);
// gets the raw value for the port, must be null terminated
void SERIAL_uartSend(char const * const  pTransmitString);
// get a char from the serial buffer
char SERIAL_uartGetSync(void);
// set the async handler
void SERIAL_uartAsyncGetHandler(asyncGetHandler_t cb);


#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // Serial_h_INCLUDED