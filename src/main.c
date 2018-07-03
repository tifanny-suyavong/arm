#include "gpios.h"
#include "interrupts.h"
#include "uart.h"

// Reset and clock controller
#define RCC_BASE     0x40023800
#define RCC_CFGR     (RCC_BASE + 0x08)
#define RCC_AHB1ENR  (RCC_BASE + 0x30)
#define RCC_APB2ENR  (RCC_BASE + 0x44)

#define BAUDRATE 115200

static void enable_usart(void)
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

static void enable_usart_transmitter(void)
{
  // Set TE bit to send idle frame as first transmission
  volatile int *usart1_cr1 = (volatile int *)USART1_CR1;
  *usart1_cr1 |= 0x8;
}

static void enable_usart_receiver(void)
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
//    volatile int *usart_dr = (volatile int *)USART1_DR;
    volatile char *usart_dr = ((volatile char *)USART1_DR) + 3;
    *usart_dr |= *buf;
  }

  // Wait for TC=1
  while (!(*usart_sr & 0x40))
    continue;
}

int main(void)
{
  // Enable GPIO[A, B, G] and CRC clock
  volatile int *rcc_ahb1enr = (volatile int *)RCC_AHB1ENR;
  *rcc_ahb1enr = (*rcc_ahb1enr | 0x1043) & 0xFFFFF843;

  // Enable USART1
  volatile int *rcc_apb2enr = (volatile int *)RCC_APB2ENR;
  *rcc_apb2enr = *rcc_apb2enr | 0x10;

  // Set GPIO mode
  set_moder_mode((volatile int *)GPIOB_MODER, 6, MODER_ALTERNATE);
  set_moder_mode((volatile int *)GPIOB_MODER, 7, MODER_ALTERNATE);
  set_moder_mode((volatile int *)GPIOG_MODER, 13, MODER_OUTPUT);

  // Enable interrupts in Cortex m4
  volatile int *nvic_iser0 = (volatile int *)NVIC_ISER0;
  *nvic_iser0 |= 1 << 6;

  // Set PB6/PB7 to pull-up
  set_moder_mode((volatile int*)GPIOB_PUPDR, 6, 1);
  set_moder_mode((volatile int*)GPIOB_PUPDR, 7, 1);

  enable_button_interrupt();

  // Enable UART
  enable_usart();

//  enable_usart_receiver();

  enable_usart_transmitter();
  char buf[] = "Hello World";
  while (1)
    send_usart_data(buf, sizeof (buf));
  //send_usart_data("\xFF", 1);

  // Read char (should be done with interrupt)
  volatile int *usart_dr = (volatile int *)USART1_DR;
  volatile int *usart_sr = (volatile int *)USART1_SR;
  while (!(*usart_sr & 0x20))
    continue;

  while (1)
    continue;

  return 0;
}
