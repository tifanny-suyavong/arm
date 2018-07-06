#ifndef _UART_H
#define _UART_H

#define USART1_BASE  0x40011000
#define USART1_CR1   (USART1_BASE + 0x0C)
#define USART1_CR2   (USART1_BASE + 0x10)
#define USART1_CR3   (USART1_BASE + 0x14)
#define USART1_BRR   (USART1_BASE + 0x08)
#define USART1_DR    (USART1_BASE + 0x04)
#define USART1_SR    (USART1_BASE)

void enable_usart(void);
void enable_usart_transmitter(void);
void enable_usart_receiver(void);
void send_usart_data(const char *buf, long len);
void read_usart_data(void *dest, unsigned long count);

#endif
