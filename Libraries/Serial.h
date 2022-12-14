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

#pragma message("WARNING: this module defaults to TX0/RX0 for interrupts and serial communication")

#include <stdint.h>

/* NOTE: Custom Types */
// typing for the stepper motor enum
typedef enum SerialPortSelector_t
{
    // usart 0 selector
    serialUsart0 = 0,
    // usart 1 selector
    serialUsart1 = 1,
    // usart 2 selector
    serialUsart2 = 2,
    // usart 3 selector
    serialUsart3 = 3,
} SerialPortSelector_t;

// typing for the handler function
typedef void (*SerialAsyncGetHandler_t)(char);

/* NOTE: Function prototypes */
// init for the serial sync mode
void SERIAL_uartInit(SerialPortSelector_t port, uint32_t baud);
// init for the serial async mode
void SERIAL_uartInitAsync(SerialPortSelector_t port, uint32_t baud);
// sends the buffer to the desired port
// the string must be null terminated
void SERIAL_uartSend(SerialPortSelector_t port, char const * const pTransmitString);
// sends the buffer to the desired port
void SERIAL_uartSendFixed(SerialPortSelector_t port, char const * const pTransmitString, uint16_t length);
// get a char from the serial buffer
char SERIAL_uartGetSync(SerialPortSelector_t port);
// set the async handler
// will run on every character to the uart buffer
void SERIAL_uartAsyncGetHandler(SerialPortSelector_t port, SerialAsyncGetHandler_t cb);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif // Serial_h_INCLUDED