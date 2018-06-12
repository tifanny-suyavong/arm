#ifndef _UART_H
#define _UART_H

#define GPIOB_MODER  0x40020400
#define USART1_BASE  0x40011000
#define USART1_CR1   (USART1_BASE + 0x0C)
#define USART1_CR2   (USART1_BASE + 0x10)
#define USART1_CR3   (USART1_BASE + 0x14)
#define USART1_BRR   (USART1_BASE + 0x08)
#define USART1_DR    (USART1_BASE + 0x04)
#define USART1_SR    (USART1_BASE)

#endif