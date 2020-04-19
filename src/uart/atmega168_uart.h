/**
 * @file atmega168_uart.c
 *
 * @brief 
 * ATmega168 driver code. This code implements an interface to control the ATmega168's
 * USART peripheral.
 */
#ifndef _ATMEGA168_UART_H
#define _ATMEGA168_UART_H

typedef int BOOL;
enum {TRUE = 1, FALSE = 0};

void create(uint32_t baud_rate);
void destroy(void);
BOOL is_send_ready(void);
void send(uint8_t data);
BOOL is_data_available(void);
void flush(void);
uint8_t receive(void);
extern void uart_rx_complete_ISR(void);

#endif /*_ATMEGA168_UART_H*/
