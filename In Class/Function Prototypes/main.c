/*
 * Function Prototypes.c
 *
 * Created: 9/2/2022 9:00:33 AM
 * Author : Ethan Zeronik
 */

#include <avr/io.h>

uint16_t retCal(char, uint8_t);

int main(void)
{
    /* Replace with your application code */
    while(1)
    {
    }
}

uint16_t retCal(char a, uint8_t b)
{
    uint8_t c = 150;
    uint8_t d = 200;

    uint16_t result = 0;

    switch(a)
    {
        case 'W':
        {
            result = b + c + d;
        }
        break;
        case 'H':
        {
            result = (d / c) + b;
        }
        break;
        default:
        {
            result = 0xff;
        }
        break;
    }

    return result;
}
