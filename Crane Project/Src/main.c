/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 11/11/2022 8:46:12 AM
 * Author: Ethan Zeronik
 *
 * Operations: crane project source code
 *
 * Hardware:
 *   Atmega2560          micro controller
 */

/* NOTE: Includes */
#include <avr/io.h>

/* NOTE: Custom Macros */
// TODO: None

/* NOTE: Global Variables */
// TODO: None

/* NOTE: Function prototypes */
// inits IO ports
void IO_init(void);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();

    while(1)
    {
        /*
            three modes, record, calibrate, and default

            calibrate should rerun the homing during init

            record needs to save the the actions to a LL and store them in eeprom

            action needs to be able to move the crane
        */
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // TODO: None
}

