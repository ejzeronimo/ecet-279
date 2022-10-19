/*
 * FileName: main.c
 * Version: 1
 *
 * Created: 10/14/2022 5:40:25 PM
 * Author: Ethan Zeronik
 *
 * Operations: Homework 8 final question
 *
 */

/* NOTE: Includes */
#include <avr/io.h>

#define F_CPU 16000000UL

/* NOTE: Function prototypes */
// inits IO ports
void     IO_init(void);
// inits PWM
void     PWM_init(void);
// outputs the given number to the PWM wave
void     PWM_Output(uint16_t input);
// inits ADC
void     ADC_init(void);
// gets the 10 bit value from the given ADC channel
uint16_t ADC_10bit(uint8_t channel);

/* NOTE: Application implementation */
// the main loop of the function, provided to us
int main(void)
{
    IO_init();

    ADC_init();

    uint16_t pwmAdcAdjustedValue = 0;

    while(1)
    {
        // call our logic here
        pwmAdcAdjustedValue = ((ADC_10bit(0) * 63) / 1023) + 62;

        PWM_Output(pwmAdcAdjustedValue);
    }
}

/* NOTE: Function implementations */
void IO_init(void)
{
    // pwm output
    DDRE  = 0xFF;
    PORTE = 0x00;

    // ADC input
    DDRF  = 0x00;
    PORTF = 0xFF;
}

void PWM_init(void)
{
    // set counter to 0
    TCNT3 = 0;

    // set up as clear on match
    TCCR3A = 0x83;

    // fast PWM custom top mode
    TCCR3B = 0x18;

    TCCR3C = 0x00;

    // set to max of 1249 for 50hz
    OCR3A = 1249;
}

void PWM_Output(uint16_t input)
{
    TCNT3 = input;

    TCCR3B |= 0x7; // prescale
}

void ADC_init(void)
{
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);

    // 5V reference
    ADMUX = (1 << REFS0);

    ADCSRB = 0x00;
}

uint16_t ADC_10bit(uint8_t channel)
{
    uint16_t result = 0;

    // select channel
    ADMUX = (ADMUX & 0xE0) | channel;

    // start conversion
    ADCSRA |= (1 << ADSC);

    // wait until
    while((ADCSRA & (1 << ADSC)) == 1)
    {
        // do nothing
    }

    // store 10 bit result
    result = ADCL;
    result = result | (ADCH << 8);

    return result;
}
