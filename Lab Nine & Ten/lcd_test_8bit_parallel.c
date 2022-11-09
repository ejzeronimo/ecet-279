/****************************************************/
/*													*/
/* Project name:	LCD_Test_8bit_parallel			*/
/* Displays a two line test message:				*/
/*													*/
/*		Test message: LCD is OK						*/
/*		in 8bit parallel mode						*/
/*   												*/
/* on the 2x16 dot matrix LCD at a specific 		*/
/* position using the ATMEL ATmega2560				*/
/*													*/
/* PORTL for LCD data:							 	*/
/* 													*/
/* L.0  LCD D0								        */
/* L.1  LCD D1 								        */
/* etc.									    		*/
/* L.7  LCD D7								        */
/*													*/
/* PORTD for LCD control:							*/
/*													*/
/*	D.0	LCD RS							         	*/
/*	D.1	LCD R/!W						        	*/
/*	D.2	LCD E							            */
/*													*/
/* 	The code is written as a single .c module		*/
/*													*/
/*	Author:		Prof A Oxtoby		*/
/*	Modified:	April Cheung						*/
/*	Date:		3-13-17								*/
/*	Ver:		1.0									*/
/*	Filename:	LCD_8bit_parallel.c		*/
/*													*/
/****************************************************/

/* #defines */

#define F_CPU 16000000UL // F_CPU must be defined BEFORE the #include <util/delay.h>

/*includes*/
#include <avr/io.h>
#include <util/delay.h>

#define INSTR_WR	0
#define DATA_WR		1

/* declare function prototypes */

void Init_PORTs (void);
void Init_LCD (void);
void LCD_write (unsigned char RS, unsigned char data);
void Print_string(char *data_ptr);

int main(void)
{
	char T_mess[] = {"Test message: "};
	char B_mess1[] = {"LCD is OK!           "};
	char B_mess2[] = {"8bit parallel"};

	/* MUST initialize PORTs BEFORE initializing LCD */
	/* since need to write to LCD to initialize it */
	Init_PORTs();
	Init_LCD();


	LCD_write(INSTR_WR, 0x01);
	_delay_ms(2);	//can remove if use Busy Flag check

	LCD_write(INSTR_WR, 0x02);
	 _delay_ms(2);  //can remove if use Busy Flag check

	//LCD_write(INSTR_WR, 0x83);
	 Print_string (T_mess);


	LCD_write(INSTR_WR, 0xC0);
	_delay_us(50);	//can remove if use Busy Flag check


	while(1) /* start of super loop */
	{

	LCD_write(INSTR_WR, 0xC0);	// Set DDRAM address to bottom row, left character
	_delay_us(50);	//can remove if use Busy Flag check
	Print_string (B_mess1);
	_delay_ms(2000);
	LCD_write(INSTR_WR, 0xC0);	// Set DDRAM address to bottom row, left character
	_delay_us(50);	//can remove if use Busy Flag check
	Print_string (B_mess2);
	_delay_ms(2000);

	}  /*end while(1) loop */

	return(0);

}	/* end main */

void Init_PORTs (void)
{
	DDRL = 0xFF;	/* PORTC all outputs */
	PORTL = 0x00;	/* Set all outputs to 0 initially */
	DDRD = 0x07;	/* Set D.0 through A.2 to outputs for LCD control */
}

					/* Perform LCD initialization */
void Init_LCD (void)
{
	_delay_ms(35);				/* wait for more than 30mS after VDD rises to 4.5V */
	 LCD_write(INSTR_WR,0x38); 	/* function set 8bits, 2line, display off */
	_delay_us(50);				/* wait for more than 39microS */
	LCD_write(INSTR_WR,0x0C);	/* display on, cursor off, blink off */
	_delay_us(50);				/* wait for more than 39microS */
	LCD_write(INSTR_WR,0x01);	/* display clear */
	_delay_ms(2);				/* wait for more than 1.53mS */
	LCD_write(INSTR_WR,0x06);	/* entry mode set, increment mode */
}
	/* End of LCD initialization */

void LCD_write (unsigned char RS, unsigned char data)
{
	if(RS==DATA_WR) PORTD = 0b00000001;	/* write data: E = 0 R/!W=0, (write)RS = 1 ,   */
	else			PORTD = 0b00000000;	/* Write instruction: RS = 0 E = 0, R/!W=0 (write) */


	PORTD = PORTD | 0x04;	/* Take E HIGH (logic 1) */
	PORTL = data;
	_delay_us(50);			/* needs to be at least 30uS or no display - use 50 */
	PORTD = PORTD & 0x01;	/* Take E LOW (logic 0) */
	_delay_us(50);			/* Delay REQUIRED */
}

void Print_string(char *str_ptr)
{
	PORTD = 0b00000001;	/* write data: RS = 1 E = 0, R/!W=0 (write) */

	while(*str_ptr != '\0')
	{
		PORTD = PORTD | 0x04;	/* Take E HIGH (logic 1) */
		PORTL = *str_ptr++;
		_delay_us(50);			/* needs to be at least 30uS or no display - use 50 */
		PORTD = PORTD & 0x01;	/* Take E LOW (logic 0) */
		_delay_us(50);			/* Delay REQUIRED */
	}
}


