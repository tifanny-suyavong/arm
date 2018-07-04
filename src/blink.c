#include "gpios.h"
#include "interrupts.h"

// Reset and clock controller
#define RCC_BASE     0x40023800
#define RCC_CFGR     (RCC_BASE + 0x08)
#define RCC_AHB1ENR  (RCC_BASE + 0x30)
#define RCC_APB2ENR  (RCC_BASE + 0x44)

int main(void)
{
  // Enable GPIO[A, B, G] and CRC clock
  volatile int *rcc_ahb1enr = (volatile int *)RCC_AHB1ENR;
  *rcc_ahb1enr = (*rcc_ahb1enr | 0x1043) & 0xFFFFF843;

  set_moder_mode((volatile int *)GPIOG_MODER, 13, MODER_OUTPUT);

  int on = 0;
  while (1)
  {
    for (int wait = 0; wait < 100000; ++wait)
      continue;

    power_led(on);
    on = !on;
  }

  return 0;
}
