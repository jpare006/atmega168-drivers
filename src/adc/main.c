#ifndef F_CPU
#define F_CPU 8000000UL //assumes fuses configured for 8MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>

#include "atmega168_adc.h"

int main(void)
{
	DDRD = 0xFF;
	PORTD = 0x00;
	create();

	while(1)
	{
		PORTD = read();

		_delay_ms(200);
	}

	return 1;
}