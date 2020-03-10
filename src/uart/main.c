#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include "uart.h"
#include <util/delay.h>
#include <avr/io.h>

char string[] = "hello, world! I am your creator.\n";

//receive string from arduino then send it back
int main(void)
{
	DDRB = 0x01;
	PORTB = 0x00;
	
	char dataBuffer[CBUF_SIZE] = {};
	int size_read = 0;

	uart_init(9600);
	_delay_ms(450);

	while (1)
	{
		size_read = uart_read_string(dataBuffer, KEYBOARD);
		uart_send_string(dataBuffer, size_read);
	}
}

//=================== pair ===============================
//send string
// int main(void)
// {
// 	DDRB = 0x01;
// 	PORTB = 0x00;

// 	uart_init(9600);

// 	_delay_ms(450);

// 	while (1)
// 	{
// 		if(uart_send_ready())
// 		{
// 			size_t size = sizeof(string);
// 			uart_send_string(string, size);

// 			//toggle ligh to indicate send was made
// 			PORTB = 0x01;
// 			_delay_ms(1000);
// 			PORTB = 0x00;
// 			_delay_ms(1000);
// 		}
// 	}
// }

// //receive string and then send string to arduino
// int main(void)
// {
// 	DDRB = 0x01;
// 	PORTB = 0x00;
	
// 	char dataBuffer[CBUF_SIZE];
// 	size_t size_read = 0;

// 	uart_init(9600);
// 	_delay_ms(450);

// 	while (1)
// 	{
// 		size_read = uart_read_string(dataBuffer, MCU);

// 		uart_send_string(dataBuffer, size_read);
// 	}
// }
//============================================================