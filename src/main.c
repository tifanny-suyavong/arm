#include "interrupts.h"
#include "uart.h"

// MODER possible values
#define MODER_INPUT 0
#define MODER_OUTPUT 1
#define MODER_ALTERNATE 2

// Reset and clock controller
#define RCC_AHB1ENR  (0x40023800 + 0x30)

// Offset input/output
#define IDR_OFF      0x10
#define ODR_OFF      0x14

// User button : pin A0
#define GPIOA_MODER  0x40020000
#define GPIOA_IDR    (GPIOA_MODER + IDR_OFF)

// Led LD3 : pin G13
#define GPIOG_MODER  0x40021800
#define GPIOG_ODR    (GPIOG_MODER + ODR_OFF)

void power_led(int on)
{
  volatile int *led_output_data = (volatile int *)GPIOG_ODR;

  if (on)
    *led_output_data = *led_output_data | (1 << 13);
  else
    *led_output_data = *led_output_data & ~(1 << 13);
}

void interrupt_handler(void)
{
  static int led_status = 0;
  led_status = !led_status;

  volatile int *exti_pr = (volatile int *)EXTI_PR;
  *exti_pr |= 1;
  volatile int *nvic_icpr0 = (volatile int *)NVIC_ICPR0;
  *nvic_icpr0 |= 1 << 6;

  power_led(led_status);
}

static void set_moder_mode(volatile int *gpio_moder, int pos, int mode)
{
  *gpio_moder = (*gpio_moder & ~(3 << (pos * 2))) | (mode << (pos * 2));
}

int main(void)
{
    // Enable GPIO[A, B, G] and CRC clock
    volatile int *rcc = (volatile int *)(RCC_AHB1ENR);
    *rcc = (*rcc | 0x1043) & 0xFFFFF843;

    // Set GPIO mode
    set_moder_mode((volatile int *)GPIOA_MODER, 0, MODER_INPUT);
    set_moder_mode((volatile int *)GPIOB_MODER, 6, MODER_ALTERNATE);
    set_moder_mode((volatile int *)GPIOB_MODER, 7, MODER_ALTERNATE);
    set_moder_mode((volatile int *)GPIOG_MODER, 13, MODER_OUTPUT);

    // Enable external interrupt for PA0 (user button)
    volatile int *exticr1 = (volatile int *)SYSCFG_EXTICR1;
    *exticr1 &= 0xFFFFFFF0;

    // Unmask interrupt for line 0
    volatile int *extimr = (volatile int *)EXTI_IMR;
    *extimr |= 0x1;

    // Set rising/falling triggers for line 0
    volatile int *extrstr = (volatile int *)EXTI_RSTR;
    volatile int *extfstr = (volatile int *)EXTI_FSTR;
    *extrstr |= 0x1;
    *extfstr |= 0x1;

    // Enable interrupts in Cortex m4
    volatile int *nvic_iser0 = (volatile int *)NVIC_ISER0;
    *nvic_iser0 |= 1 << 6;

    while (1)
      continue;

    return 0;
}
