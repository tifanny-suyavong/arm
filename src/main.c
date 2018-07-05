#include "flash.h"
#include "gpios.h"
#include "interrupts.h"
#include "uart.h"

// Reset and clock controller
#define RCC_BASE     0x40023800
#define RCC_CFGR     (RCC_BASE + 0x08)
#define RCC_AHB1ENR  (RCC_BASE + 0x30)
#define RCC_APB2ENR  (RCC_BASE + 0x44)

void echo(void)
{
  char c = 0;

  volatile int *usart_dr = (volatile int *)USART1_DR;
  volatile int *usart_sr = (volatile int *)USART1_SR;

  while (1)
  {
    while (!(*usart_sr & 0x20))
      continue;

    c = *usart_dr;
    send_usart_data(&c, 1);
  }
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

  unlock();
  erase_sector(1);
  char mybuf[] = "Tifanny Suyavong et Guillaume Taquet Gasperini font le projet";
  write_to_flash(mybuf, sizeof (mybuf), (char*)0x08004000);
  lock();

  // Enable UART
  enable_usart();

  enable_usart_receiver();
  enable_usart_transmitter();

  void(*fun)(void);
  fun = (void*)(0x08004000);
  fun();

  echo();
/*  char buf[] = "Hello World";
  while (1)
    send_usart_data(buf, sizeof (buf));
*/
  // Read char (should be done with interrupt)
  volatile int *usart_dr = (volatile int *)USART1_DR;
  volatile int *usart_sr = (volatile int *)USART1_SR;
  while (!(*usart_sr & 0x20))
    continue;

  while (1)
    continue;

  return 0;
}
