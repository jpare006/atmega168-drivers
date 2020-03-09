/**
 * @file main.c
 *
 * @brief 
 * Shows example usage of timer library
 * The code simply toggles and entire port every 500ms
 */

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include "timer.h"

int main(void)
{
	int toggle = 1;

	DDRD = 0xFF;
	PORTD = 0x00;

	timer_init(500);
	timer_on();

	while(1)
	{
		if (toggle)
		{
			PORTD = 0xFF;
			toggle = 0;
		}
		else
		{
			PORTD = 0x00;
			toggle = 1;
		}

		while(!TimerFlag);
		TimerFlag = 0;
	}
}

/*** end of file ***/