#include "gpios.h"
#include "uart.h"

#define BAUDRATE 115200

void enable_usart(void)
{
  // Set alternate mode (AF7 = 0b0111) for PB6 and PB7
  volatile int *gpiob_afrl = (volatile int *)GPIOB_AFRL;
  *gpiob_afrl = (*gpiob_afrl | 0x77000000);

  volatile int *usart1_cr1 = (volatile int *)USART1_CR1;

  // Enable USART
  // Set UE and M (8 bits)
  *usart1_cr1 = (*usart1_cr1 | 0x2000) & 0xFFFFEFFF;

  // Disable parity
  *usart1_cr1 &= 0xFFFFFBFF;

  // Set 1 stop bit
  volatile int *usart1_cr2 = (volatile int *)USART1_CR2;
  *usart1_cr2 &= 0xFFFFCFFF;

  // Disable DMAT
  volatile int *usart1_cr3 = (volatile int *)USART1_CR3;
  *usart1_cr3 &= 0xFFFFFF7F;

  // Configure baud rate for 115.2KBps
  // 16Mhz, Over8=0, USARTDIV=8.6875
  // USARTDIV = DIV_Mantissa + DIV_Fraction/16
  // DIV_Mantisa=8; DIV_Fraction=0.6875*16=11
  volatile int *usart1_brr = (volatile int *)USART1_BRR;
  int mantissa = 8;
  int fractional = 11;
  // compute_brr_parts(BAUDRATE, &mantissa, &fractional);
  *usart1_brr = (*usart1_brr & 0xFFFF0000) | (mantissa << 4) | fractional;
}

void enable_usart_transmitter(void)
{
  // Set TE bit to send idle frame as first transmission
  volatile int *usart1_cr1 = (volatile int *)USART1_CR1;
  *usart1_cr1 |= 0x8;
}

void enable_usart_receiver(void)
{
  // Set RE bit
  volatile int *usart1_cr1 = (volatile int *)USART1_CR1;
  *usart1_cr1 |= 0x4;
}

void send_usart_data(const char *buf, long len)
{
  volatile int *usart_sr = (volatile int *)USART1_SR;

  for (; len > 0; --len, ++buf)
  {
    // Wait for TXE=1
    while (!(*usart_sr & 0x80))
      continue;

    // Write data
    volatile char *usart_dr = ((volatile char *)USART1_DR) + 3;
    *usart_dr |= *buf;
  }

  // Wait for TC=1
  while (!(*usart_sr & 0x40))
    continue;
}

void read_usart_data(void *dest, unsigned long count)
{
  char *buf = dest;

  volatile int *usart_dr = (volatile int *)USART1_DR;
  volatile int *usart_sr = (volatile int *)USART1_SR;

  for (; count > 0; --count, ++buf)
  {
    while (!(*usart_sr & 0x20))
      continue;

    *buf = *usart_dr;
  }
}
