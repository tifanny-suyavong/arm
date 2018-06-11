#include "interrupts.h"

#define RCC_AHB1ENR  (0x40023800 + 0x30)         // Reset and clock controller

#define IDR_OFF      0x10
#define ODR_OFF      0x14

// User button : pin A0
#define GPIOA_MODER  0x40020000
#define GPIOA_IDR    (GPIOA_MODER + IDR_OFF)

// Led LD3 : pin G13
#define GPIOG_MODER  0x40021800
#define GPIOG_ODR    (GPIOG_MODER + ODR_OFF)

#define SET_MODER_INPUT(A, POS)                         \
    (A & ~(1 << (POS * 2 + 1))) & ~(1 << (POS * 2))
#define SET_MODER_OUTPUT(A, POS)                        \
    (A & ~(1 << (POS * 2 + 1))) | (1 << (POS * 2))

void manage_led(int turn_on)
{
  volatile int *led_output_data = (volatile int *) GPIOG_ODR;

  if (turn_on)
    *led_output_data = *led_output_data | (1 << 13);
  else
    *led_output_data = *led_output_data & ~(1 << 13);
}

void interrupt_handler(void)
{
  static int status = 0;
  status = !status;

  volatile int *exti_pr = (volatile int *) EXTI_PR;
  *exti_pr |= 1;
  volatile int *nvic_icpr0 = (volatile int *) NVIC_ICPR0;
  *nvic_icpr0 |= 1 << 6;

  manage_led(status);
}

int main(void)
{
    volatile int *rcc = (volatile int *) RCC_AHB1ENR;
    volatile int *gpioa_mode = (volatile int *) GPIOA_MODER;
    volatile int *gpiog_mode = (volatile int *) GPIOG_MODER;
    volatile int *exticr1 = (volatile int *) SYSCFG_EXTICR1;
    volatile int *extimr = (volatile int *) EXTI_IMR;
    volatile int *extrstr = (volatile int *) EXTI_RSTR;
    volatile int *extfstr = (volatile int *) EXTI_FSTR;
    volatile int *nvic_iser0 = (volatile int *) NVIC_ISER0;

    // Enable all the GPIOs
    *rcc = *rcc | 0x7FF;

    // Set GPIO mode
    *gpioa_mode = SET_MODER_INPUT(*gpioa_mode, 0);
    *gpiog_mode = SET_MODER_OUTPUT(*gpiog_mode, 13);

    // Enable external interrupt for PA0 (user button)
    *exticr1 &= 0xFFFFFFF0;

    // Unmask interrupt for line 0
    *extimr |= 0x1;

    // Set rising/falling triggers for line 0
    *extrstr |= 0x1;
    *extfstr |= 0x1;

    // Enable interrupts in Cortex m4
    *nvic_iser0 |= 1 << 6;

    while (1)
    {
        /* if (*button_input_data & 0x1) */
        /*     *led_output_data = *led_output_data | (1 << 13); */
        /* else */
        /*     *led_output_data = *led_output_data & ~(1 << 13); */
    }
    return 0;
}
