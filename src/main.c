#include "flash.h"
#include "gpios.h"
#include "interrupts.h"
#include "uart.h"

// Reset and clock controller
#define RCC_BASE     0x40023800
#define RCC_CFGR     (RCC_BASE + 0x08)
#define RCC_AHB1ENR  (RCC_BASE + 0x30)
#define RCC_APB2ENR  (RCC_BASE + 0x44)

#define PAGE_SIZE 16384
#define FLASH_ADDR 0x08008000
//#define FLASH_BIN_SIZE 588

/*void read_and_flash(void* flash_addr)
{
  unsigned long long count = 0;
  read_usart_data(&count, 1);

  char buf[PAGE_SIZE];
  write_to_flash(buf, count, flash_addr);
}
*/
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

  // Enable UART
  enable_usart();

  enable_usart_receiver();
  enable_usart_transmitter();

  char buf[PAGE_SIZE];
  read_usart_data(buf, FLASH_BIN_SIZE);
  unlock();
  erase_sector(2);
  write_to_flash(buf, FLASH_BIN_SIZE, (void*)FLASH_ADDR);
  lock();

  void(*fun)(void);
  fun = (void*)(FLASH_ADDR + 0x1ac);
  fun();

//  echo();
/*  char buf[] = "Hello World";
  while (1)
    send_usart_data(buf, sizeof (buf));
*/

  while (1)
    continue;

  return 0;
}
